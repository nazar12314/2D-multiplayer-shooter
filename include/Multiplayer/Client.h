#pragma once

#include <boost/asio.hpp>
#include "net_modules.h"

template <typename T> class Client
{
	boost::asio::io_context io_context;
	tcp::socket socket;
	net::Message<T> message;

	boost::lockfree::queue<net::Message<T>*> message_queue {1024};
	std::shared_ptr<net::Connection<T>> connection;

	std::thread io_context_thread;

public:
	int id = -1;

	Client(const std::string& host, const std::string& port) : socket(io_context)
	{
		connect(host, port);
	}

	void connect(const std::string& host, const std::string& port)
	{
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(host, port);
		boost::asio::connect(socket, endpoints);

		connection = std::make_shared<net::Connection<T>>(std::move(socket), message_queue);
		connection->start();

		io_context_thread = std::thread([this] { io_context.run(); });
	}

	template <typename DataType> void send_message(T msg_type, const DataType& msg_body)
	{
		message.header.id = msg_type;
		message.set_body(msg_body);

		connection->write(message);
	}

	void close()
	{
		socket.close();
	}

	friend class Multiplayer;
};
