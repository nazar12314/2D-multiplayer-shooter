#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <deque>
#include <thread>

using boost::asio::ip::tcp;

namespace net {
    template <typename T>
    struct MessageHeader
    {
        T id {};
        uint32_t size = 0;
    };

    template <typename T>
    struct Message
    {
        MessageHeader<T> header {};
        std::vector<uint8_t> body;

        Message() = default;
        Message(const Message<T>& other)
                :
                header(other.header), body(other.body) {}

        size_t full_size() const {
            return sizeof(MessageHeader<T>) + body.size();
        }

        template <typename DataType>
        void set_body(const DataType& data) {
            body.clear();
            body.resize(sizeof(DataType));
            std::memcpy(body.data(), &data, sizeof(DataType));
            header.size = static_cast<uint32_t>(body.size());
        }

        template <typename DataType>
        DataType get_body() {
            DataType result;
            std::memcpy(&result, body.data(), sizeof(DataType));
            return result;
        }
    };

    struct ObjectDescription {
        int id;
        std::string name;

        glm::vec2 position;
        float rotation;
    };

    enum class MessageType : uint32_t
    {
        SERVER_STATUS,
        ADD_PLAYER,
        REMOVE_PLAYER,
        UPDATE_PLAYER,
        PLAYER_MESSAGE
    };

    template <typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>> {

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
        boost::lockfree::queue<Message<T>*>& messages_queue;

    public:
        explicit Connection(tcp::socket socket, boost::lockfree::queue<Message<T>*>& queue, owner parent_type = owner::server)
                :
                socket(std::move(socket)), messages_queue(queue), parent_type(parent_type) {}

        void start()
        {
            read_header();
        }

        void add_message_to_queue()
        {
            if (parent_type == owner::server)
            {
                auto msg_copy = new Message<T>(read_msg_);
                messages_queue.push(msg_copy);
            }

            read_header();
        }

        void read_header()
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
                                        } else
                                        {
                                            socket.close();
                                        }
                                    });
        }

        void read_body()
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

        void write(const Message<T>& msg)
        {
            bool write_in_progress = !write_msgs.empty();
            write_msgs.push_back(msg);

            if (!write_in_progress)
            {
                write_header();
            }
        }

        void write_header()
        {
            auto self(this->shared_from_this());

            boost::asio::async_write(socket, boost::asio::buffer(&write_msgs.front().header, sizeof(MessageHeader<T>)),
                                     [this, self](boost::system::error_code ec, std::size_t length)
                                     {
                                         if (!ec)
                                         {
                                             write_body();
                                         } else
                                         {
                                             std::cerr << "Error on header write: " << ec.message() << std::endl;
                                             socket.close();
                                         }
                                     });
        }

        void write_body()
        {
            auto self(this->shared_from_this());

            boost::asio::async_write(socket, boost::asio::buffer(write_msgs.front().body.data(), write_msgs.front().body.size()),
                                     [this, self](boost::system::error_code ec, std::size_t length)
                                     {
                                         if (!ec) {
                                             write_msgs.pop_front();
                                             if (!write_msgs.empty()) {
                                                 write_header();
                                             }
                                         } else {
                                             std::cerr << "Error on body write: " << ec.message() << std::endl;
                                             socket.close();
                                         }
                                     });
        }

        bool is_connected() const
        {
            return socket.is_open();
        }
    };
}