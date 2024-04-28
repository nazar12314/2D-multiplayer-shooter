#pragma once

#include <boost/asio.hpp>
#include "net_modules.h"

class Client
{
	boost::asio::io_context io_context;
	tcp::socket socket;
	net::Message<net::MessageType> message;

public:
	int id = -1;

	Client(const std::string& host, const std::string& port) :
		socket(io_context)
	{
		connect(host, port);
	}

	void connect(const std::string& host, const std::string& port)
	{
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(host, port);
		boost::asio::connect(socket, endpoints);
	}

	template <typename DataType> void send_message(net::MessageType msg_type, const DataType& message_body)
	{
		message.header.id = msg_type;
		message.set_body(message_body);

		auto header_buffer = boost::asio::buffer(&message.header, sizeof(message.header));
		auto body_buffer = boost::asio::buffer(message.body.data(), message.body.size());

		std::vector<boost::asio::const_buffer> buffers;
		buffers.emplace_back(header_buffer);
		buffers.emplace_back(body_buffer);

		boost::asio::write(socket, buffers);
	}

	void close()
	{
		socket.close();
	}
};
