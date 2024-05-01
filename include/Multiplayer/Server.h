#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <memory>
#include <set>
#include <thread>
#include "net_modules.h"

template <typename T> class Server
{
	inline static int available_id = 1;

	boost::asio::io_context context;
	//std::unique_ptr<boost::asio::io_context::work> work;
	tcp::acceptor acceptor;
	boost::lockfree::queue<net::OwnedMessage<T>*> message_queue {1024};
	std::set<std::shared_ptr<net::Connection<T>>> connections;
	std::thread io_context_thread;

	void start_accept()
	{
		acceptor.async_accept([this](const boost::system::error_code& ec, tcp::socket socket)
		{
			if (!ec)
			{
				auto connection = std::make_shared<net::Connection<T>>(context, std::move(socket), message_queue);

				connections.insert(connection);
				connection->start(available_id++);
			}
			start_accept();
		});
	}

public:
	explicit Server(short port): acceptor(context, tcp::endpoint(tcp::v4(), port))
	{
		//work = std::make_unique<boost::asio::io_context::work>(io_context);
		start_accept();
		io_context_thread = std::thread([this] { context.run(); });
	}

	~Server()
	{
		//work.reset();

		if (io_context_thread.joinable())
			io_context_thread.join();
	}

	void message_client(std::shared_ptr<net::Connection<T>> client, net::Message<T>& msg)
	{
		if (!client->is_connected())return;
		client->write(msg);
	}
	template <typename DataType> void message_client(std::shared_ptr<net::Connection<T>> client, T msg_type, const DataType& message_body)
	{
		net::Message<T> msg;
		msg.header.id = msg_type;
		msg.set_body(message_body);

		message_client(client, msg);
	}

	void message_clients(net::Message<T>& msg, std::shared_ptr<net::Connection<T>> ignore_client = nullptr)
	{
		for (auto& connection : connections)
		{
			if (connection == ignore_client) continue;

			message_client(connection, msg);
		}
	}
	template <typename DataType> void message_clients(T msg_type, const DataType& message_body, std::shared_ptr<net::Connection<T>> ignore_client = nullptr)
	{
		net::Message<T> msg;
		msg.header.id = msg_type;
		msg.set_body(message_body);

		message_clients(msg, ignore_client);
	}


	friend class Multiplayer;
};
