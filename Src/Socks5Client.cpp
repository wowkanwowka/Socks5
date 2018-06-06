//
// Created by Vladimir on 31.05.2018.
//

#include "Socks5Client.h"
#include <glog/logging.h>

#include <iostream>

Socks5ClientConnection::Socks5ClientConnection
        (boost::asio::io_service &service,
         const boost::asio::ip::tcp::endpoint &endpoint)
        : service_(service), endpoint_(endpoint), socket_(service) {}

void Socks5ClientConnection::Start() {
    if (!is_started_) {
        socket_.async_connect(endpoint_,
                                 boost::bind(&Socks5ClientConnection::OnStart, shared_from_this(), _1));
    }
}

void Socks5ClientConnection::OnStart(const boost::system::error_code& err) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        LOG(INFO) << "successfully connected\n";
        SendConnectionRequest();
//        Stop();
    }
}

void Socks5ClientConnection::Stop() {
    if (is_started_) {
        is_started_ = false;
        socket_.close();
    }
}

Socks5Client::Socks5Client(boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& endpoint)
        : service_(service), endpoint_(endpoint) {}

void Socks5Client::Start() {
    connection_ = boost::make_shared<Socks5ClientConnection>(service_, endpoint_);
    connection_->Start();
}

void Socks5Client::Stop() {
    connection_->Stop();
    connection_.reset();
}

void Socks5ClientConnection::SendConnectionRequest() {
    size_t message_size = Utilities::MakeConnectionRequest<WriteBufferSize>(&write_buffer_);
    socket_.async_send(boost::asio::buffer(write_buffer_, message_size),
                       boost::bind(&Socks5ClientConnection::OnSendConnectionRequest, shared_from_this(), _1, _2));
}

void Socks5ClientConnection::OnSendConnectionRequest(const boost::system::error_code& err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        LOG(INFO) << "successfully sent connection request\n";
        ReceiveConnectionRequestResponse();
    }
}

void Socks5ClientConnection::ReceiveConnectionRequestResponse() {
    boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_, 2),
                            boost::bind(&Socks5ClientConnection::OnReceiveConnectionRequestResponse,
                                        shared_from_this(), _1, _2));
}

void Socks5ClientConnection::OnReceiveConnectionRequestResponse(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        LOG(INFO) << "successfully received connection request response\n";
        for (size_t i = 0; i < bytes; ++i) {
            std::cout << std::bitset<8>(read_buffer_[i]) << std::endl;
        }
        std::cout << std::endl;
//        SendAuthenticationRequest();
    }
}

void Socks5ClientConnection::SendAuthenticationRequest() {
    size_t message_size = MakeAuthenticationMessage(&write_buffer_);
    boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_, message_size),
            boost::bind(&Socks5ClientConnection::OnSendAuthenticationRequest, shared_from_this(), _1, _2));
}

void Socks5ClientConnection::OnSendAuthenticationRequest(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        LOG(ERROR) << err.message() << '\n';
        Stop();
    } else {
        ReceiveAuthenticationRequestResponse();
    }
}
