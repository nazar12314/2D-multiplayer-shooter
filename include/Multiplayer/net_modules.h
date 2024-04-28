#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <memory>
#include <deque>

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

	struct Data
	{
		int id;
	};

	struct PlayerGameData : Data
	{
		glm::vec2 position;
		float rotation;
	};

	struct PlayerConnectionData : Data
	{
		char name[20];
	};

	enum class MessageType : uint32_t
	{
		SERVER_STATUS,
		ADD_PLAYER,
		REMOVE_PLAYER,
		UPDATE_PLAYER,
		PLAYER_MESSAGE
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

	private:
		tcp::socket socket;
		owner parent_type;

		std::array<char, 1024> buffer {};
		std::deque<Message<T>> write_msgs;

		Message<T> read_msg_;
		boost::lockfree::queue<OwnedMessage<T>*>& messages_queue;

	public:
		explicit Connection(tcp::socket socket, boost::lockfree::queue<OwnedMessage<T>*>& queue, owner parent_type = owner::server);

		void start();

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
	};

	template <typename T> Connection<T>::Connection(tcp::socket socket, boost::lockfree::queue<OwnedMessage<T>*>& queue, owner parent_type):
		socket(std::move(socket)), parent_type(parent_type), messages_queue(queue) {}
	template <typename T> void Connection<T>::start()
	{
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
		auto self(this->shared_from_this());

		boost::asio::async_read(socket, boost::asio::buffer(&read_msg_.header, sizeof(MessageHeader<T>)),
		                        [this, self](boost::system::error_code ec, std::size_t length)
		                        {
			                        if (!ec)
			                        {
				                        if (read_msg_.header.size > 0)
				                        {
					                        read_msg_.body.resize(read_msg_.header.size);
					                        read_body();
				                        }
				                        else
				                        {
					                        add_message_to_queue();
				                        }
			                        }
			                        else
			                        {
				                        socket.close();
			                        }
		                        });
	}
	template <typename T> void Connection<T>::read_body()
	{
		auto self(this->shared_from_this());

		boost::asio::async_read(socket, boost::asio::buffer(read_msg_.body.data(), read_msg_.body.size()),
		                        [this, self](boost::system::error_code ec, std::size_t length)
		                        {
			                        if (!ec)
			                        {
				                        add_message_to_queue();
			                        }
			                        else
			                        {
				                        std::cerr << "Error on body read: " << ec.message() << std::endl;
				                        socket.close();
			                        }
		                        });
	}
	template <typename T> void Connection<T>::write(const Message<T>& msg)
	{
		bool write_in_progress = !write_msgs.empty();
		write_msgs.push_back(msg);

		if (!write_in_progress)
		{
			write_header();
		}
	}
	template <typename T> void Connection<T>::write_header()
	{
		auto self(this->shared_from_this());

		boost::asio::async_write(socket, boost::asio::buffer(&write_msgs.front().header, sizeof(MessageHeader<T>)),
		                         [this, self](boost::system::error_code ec, std::size_t length)
		                         {
			                         if (!ec)
			                         {
				                         write_body();
			                         }
			                         else
			                         {
				                         std::cerr << "Error on header write: " << ec.message() << std::endl;
				                         socket.close();
			                         }
		                         });
	}
	template <typename T> void Connection<T>::write_body()
	{
		auto self(this->shared_from_this());

		boost::asio::async_write(socket, boost::asio::buffer(write_msgs.front().body.data(), write_msgs.front().body.size()),
		                         [this, self](boost::system::error_code ec, std::size_t length)
		                         {
			                         if (!ec)
			                         {
				                         write_msgs.pop_front();
				                         if (!write_msgs.empty())
				                         {
					                         write_header();
				                         }
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
