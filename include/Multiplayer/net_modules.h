#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <memory>
#include <deque>

#include "Color.h"
#include "glm/vec2.hpp"

using boost::asio::ip::tcp;

namespace net
{
	template <typename T> struct MessageHeader
	{
		T id {};
		uint32_t size = 0;
	};

	template <typename T> struct Message
	{
		MessageHeader<T> header {};
		std::vector<uint8_t> body;

		Message() = default;
		Message(const Message& other);

		size_t full_size() const;

		template <typename DataType> void set_body(const DataType& data);
		template <typename DataType> DataType get_body();
	};

	template <typename T> Message<T>::Message(const Message& other):
		header(other.header), body(other.body) {}
	template <typename T> size_t Message<T>::full_size() const
	{
		return sizeof(MessageHeader<T>) + body.size();
	}
	template <typename T> template <typename DataType> void Message<T>::set_body(const DataType& data)
	{
		body.clear();
		body.resize(sizeof(DataType));
		std::memcpy(body.data(), &data, sizeof(DataType));
		header.size = static_cast<uint32_t>(body.size());
	}
	template <typename T> template <typename DataType> DataType Message<T>::get_body()
	{
		DataType result;
		std::memcpy(&result, body.data(), sizeof(DataType));
		return result;
	}


	struct RegisterClientData
	{
		char name[20];
		Color color;
	};

	struct ClientAssignIdData
	{
		int id;
	};

	struct AddPlayerData
	{
		int id;
		char name[20];
		Color color;
	};

	struct PlayerRequestData
	{
		int id;
		float moveBy;
		float rotateBy;
		float rotateGunBy;
		bool shoot;
	};

	struct PlayerUpdateData
	{
		int id;
		glm::vec2 position;
		float rotation;
		float gunRotation;
		bool shoot;

		// Physics
		glm::vec2 velocity;
		float angularVelocity;
	};


	enum MessageType
	{
		REGISTER_CLIENT,
		CLIENT_ASSIGN_ID,
		ADD_PLAYER,
		REMOVE_PLAYER,
		PLAYER_ACTIONS,
		PLAYER_UPDATE,
	};

	template <typename T> struct OwnedMessage;

	template <typename T> class Connection : public std::enable_shared_from_this<Connection<T>>
	{
	public:
		enum class owner
		{
			server,
			client
		};

		int id = -1;
		tcp::socket socket;

	private:
		owner parent_type;
		boost::asio::io_context& context;

		std::array<char, 1024> buffer {};
		std::deque<Message<T>> write_msgs;

		Message<T> read_msg_;
		boost::lockfree::queue<OwnedMessage<T>*>& messages_queue;

	public:
		explicit Connection(boost::asio::io_context& context, tcp::socket socket, boost::lockfree::queue<OwnedMessage<T>*>& queue, owner parent_type = owner::server);

		void start(int id = -1);

		void add_message_to_queue();
		void read_header();
		void read_body();

		void write(const Message<T>& msg);
		void write_header();
		void write_body();

		bool is_connected() const;
	};

	template <typename T> struct OwnedMessage
	{
		Message<T>* msg;
		std::shared_ptr<Connection<T>> owner = nullptr;

		OwnedMessage(Message<T>* msg): msg(msg) {}
	};

	template <typename T> Connection<T>::Connection(boost::asio::io_context& context, tcp::socket socket, boost::lockfree::queue<OwnedMessage<T>*>& queue, owner parent_type):
		socket(std::move(socket)), parent_type(parent_type), context(context), messages_queue(queue) {}

	template <typename T> void Connection<T>::start(int id)
	{
		this->id = id;
		read_header();
	}

	template <typename T> void Connection<T>::add_message_to_queue()
	{
		auto msg_copy = new Message<T>(read_msg_);

		auto ownedMessage = new OwnedMessage<T>(msg_copy);
		if (parent_type == owner::server)
			ownedMessage->owner = this->shared_from_this();

		messages_queue.push(ownedMessage);

		read_header();
	}
	template <typename T> void Connection<T>::read_header()
	{
		boost::asio::async_read(socket, boost::asio::buffer(&read_msg_.header, sizeof(MessageHeader<T>)),
		                        [this](const boost::system::error_code& ec, std::size_t)
		                        {
			                        if (!ec)
			                        {
				                        if (read_msg_.header.size > 0)
				                        {
					                        read_msg_.body.resize(read_msg_.header.size);
					                        read_body();
				                        }
				                        else
					                        add_message_to_queue();
			                        }
			                        else
			                        {
				                        std::cerr << "Error on header read: " << ec.message() << std::endl;
				                        socket.close();
			                        }
		                        });
	}
	template <typename T> void Connection<T>::read_body()
	{
		boost::asio::async_read(socket, boost::asio::buffer(read_msg_.body.data(), read_msg_.body.size()),
		                        [this](const boost::system::error_code& ec, std::size_t)
		                        {
			                        if (!ec)
				                        add_message_to_queue();
			                        else
			                        {
				                        std::cerr << "Error on body read: " << ec.message() << std::endl;
				                        socket.close();
			                        }
		                        });
	}
	template <typename T> void Connection<T>::write(const Message<T>& msg)
	{
		boost::asio::post(context,
		                  [this, msg]
		                  {
			                  bool write_in_progress = !write_msgs.empty();
			                  write_msgs.push_back(msg);

			                  if (!write_in_progress)
				                  write_header();
		                  });
	}
	template <typename T> void Connection<T>::write_header()
	{
		boost::asio::async_write(socket, boost::asio::buffer(&write_msgs.front().header, sizeof(MessageHeader<T>)),
		                         [this](const boost::system::error_code& ec, std::size_t)
		                         {
			                         if (!ec)
				                         write_body();
			                         else
			                         {
				                         std::cerr << "Error on header write: " << ec.message() << std::endl;
				                         socket.close();
			                         }
		                         });
	}
	template <typename T> void Connection<T>::write_body()
	{
		boost::asio::async_write(socket, boost::asio::buffer(write_msgs.front().body.data(), write_msgs.front().body.size()),
		                         [this](const boost::system::error_code& ec, std::size_t)
		                         {
			                         if (!ec)
			                         {
				                         write_msgs.pop_front();
				                         if (!write_msgs.empty())
					                         write_header();
			                         }
			                         else
			                         {
				                         std::cerr << "Error on body write: " << ec.message() << std::endl;
				                         socket.close();
			                         }
		                         });
	}
	template <typename T> bool Connection<T>::is_connected() const
	{
		return socket.is_open();
	}
}
