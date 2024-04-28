#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <thread>
#include "net_modules.h"

template <typename T> class Server
{
	boost::asio::io_context io_context;
	std::unique_ptr<boost::asio::io_context::work> work;
	tcp::acceptor acceptor;
	boost::lockfree::queue<net::Message<T>*> message_queue {1024};
	std::set<std::shared_ptr<net::Connection<T>>> connections;
	std::thread io_context_thread;
	std::thread loop_thread;

	void start_accept()
	{
		acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				auto connection = std::make_shared<net::Connection<T>>(std::move(socket), message_queue);
				connections.insert(connection);
				connection->start();
			}
			start_accept();
		});
	}

public:
	explicit Server(short port) :
		acceptor(io_context, tcp::endpoint(tcp::v4(), port))
	{
		work = std::make_unique<boost::asio::io_context::work>(io_context);
		start_accept();
		io_context_thread = std::thread([this] { io_context.run(); });
	}

	~Server()
	{
		work.reset();

		if (io_context_thread.joinable())
			io_context_thread.join();

		if (loop_thread.joinable())
			loop_thread.join();
	}

	template <typename DataType> void message_clients(T msg_type, const DataType& message_body)
	{
		for (auto& connection : connections)
		{
			if (connection->is_connected())
			{
				net::Message<T> msg;
				msg.header.id = msg_type;
				msg.set_body(message_body);

				connection->write(msg);
			}
		}
	}

	void print_messages()
	{
		net::Message<T>* msg_ptr;

		while (message_queue.pop(msg_ptr))
		{
			if (msg_ptr)
			{
				//                std::cout << "Received message ID: " << msg_ptr->header.id << ", Size: " << msg_ptr->header.size << std::endl;

				auto objDesc = msg_ptr->template get_body<net::PlayerGameData>();

				std::cout << "Object ID: " << objDesc.id << std::endl;
				std::cout << "Object Name: " << objDesc.name << std::endl;
				std::cout << "Object Position: (" << objDesc.position.x << ", " << objDesc.position.y << ")" << std::endl;
				std::cout << "Object Rotation: " << objDesc.rotation << std::endl;
				std::cout << std::endl;
			}
		}
	}

	friend class Multiplayer;
};
