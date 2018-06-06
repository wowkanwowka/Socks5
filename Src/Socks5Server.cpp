//
// Created by Vladimir on 31.05.2018.
//

#include "Socks5Server.h"
#include <glog/logging.h>
#include <iostream>

Socks5ServerConnection::Socks5ServerConnection(boost::asio::io_service &service) :
        service_(service),
        from_client_to_proxy_tcp_socket_(service_),
        from_proxy_to_server_tcp_socket_(service_) {

}

boost::asio::ip::tcp::socket& Socks5ServerConnection::GetClientSocket() {
    return from_client_to_proxy_tcp_socket_;
}

void Socks5ServerConnection::Start() {
    if (!is_started_) {
        is_started_ = true;
        ReceiveConnectionRequestHead();
    }
}

void Socks5ServerConnection::Stop() {
    if (is_started_) {
        is_started_ = false;
        from_proxy_to_server_tcp_socket_.close();
        from_client_to_proxy_tcp_socket_.close();
    }
}

Socks5Server::Socks5Server(boost::asio::io_service &service, const boost::asio::ip::tcp::endpoint &endpoint) :
        service_(service), endpoint_(endpoint), acceptor_(service_, endpoint_) {}

void Socks5Server::Start() {
    Accept();
}

void Socks5Server::Stop() {
    service_.stop();
}

void Socks5Server::Accept() {
    boost::shared_ptr<Socks5ServerConnection> connection =
            boost::make_shared<Socks5ServerConnection>(service_);

    acceptor_.async_accept(connection->GetClientSocket(),
                           boost::bind(&Socks5Server::OnAccept, shared_from_this(), connection, _1));
}

void Socks5Server::OnAccept(boost::shared_ptr<Socks5ServerConnection> connection,
                            const boost::system::error_code& err) {
    if (err) {
        connection->Stop();
        LOG(ERROR) << err.message() << '\n';
    } else {
        connection->Start();
        LOG(INFO) << "new connection established " <<
                  connection->GetClientSocket().remote_endpoint().address().to_string();
    }
    Accept();
}

void Socks5ServerConnection::ReceiveConnectionRequestHead() {
    boost::asio::async_read(from_client_to_proxy_tcp_socket_, boost::asio::buffer(client_proxy_read_buffer_, 2),
                            boost::bind(&Socks5ServerConnection::OnReceiveConnectionRequestHead,
                                        shared_from_this(), _1, _2));
}

void Socks5ServerConnection::OnReceiveConnectionRequestHead(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        LOG(INFO) << "received connection request\n";
        if (bytes != 2) {
            LOG(ERROR) << "incorrect connection request head size\n";
            Stop();
            return;
        } else if (client_proxy_read_buffer_[0] != 0x05) {
            LOG(ERROR) << "incorrect socks version\n";
            Stop();
            return;
        }
        ReceiveConnectionRequestBody(static_cast<size_t>(client_proxy_read_buffer_[1]));
    }
}

void Socks5ServerConnection::ReceiveConnectionRequestBody(size_t body_length) {
    boost::asio::async_read(from_client_to_proxy_tcp_socket_, boost::asio::buffer(client_proxy_read_buffer_, body_length),
                            boost::bind(&Socks5ServerConnection::OnReceiveConnectionRequestBody,
                                        shared_from_this(), _1, _2));
}

void Socks5ServerConnection::OnReceiveConnectionRequestBody(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        LOG(INFO) << "received connection request\n";
        for (size_t i = 0; i < bytes; ++i) {
            std::cout << std::bitset<8>(client_proxy_read_buffer_[i]) << std::endl;
        }
        std::cout << std::endl;
        SendConnectionRequestResponse();
    }
}

void Socks5ServerConnection::SendConnectionRequestResponse() {
    size_t message_length =
            Utilities::MakeConnectionRequestResponse<ClientProxyWriteBufferSize>(&client_proxy_write_buffer_);
    boost::asio::async_write(from_client_to_proxy_tcp_socket_,
                             boost::asio::buffer(client_proxy_write_buffer_, message_length),
                             boost::bind(&Socks5ServerConnection::OnSendConnectionRequestResponse,
                                         shared_from_this(), _1, _2));
}
void Socks5ServerConnection::OnSendConnectionRequestResponse(const boost::system::error_code& err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << "\n";
        Stop();
    } else {
        LOG(INFO) << "successfully sent connection request response\n";
//        ReceiveAuthenticationRequest();
    }
}

void Socks5ServerConnection::ReceiveAuthenticationRequestUsername() {
//    std::string password = Utilities::GetPassword();
    boost::asio::async_read(from_client_to_proxy_tcp_socket_, boost::asio::buffer(client_proxy_read_buffer_),
                            boost::bind(&));
}

void Socks5ServerConnection::OnReceiveAuthenticationRequestUsename(const boost::system::error_code &err, size_t bytes) {

}