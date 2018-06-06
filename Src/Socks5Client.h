//
// Created by Vladimir on 31.05.2018.
//

#pragma once

#ifndef SOCKS5_SOCKS5CLIENT_H
#define SOCKS5_SOCKS5CLIENT_H

#include "Utilities.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include <array>

const size_t ReadBufferSize = 512;
const size_t WriteBufferSize = 512;

class Socks5ClientConnection : public boost::enable_shared_from_this<Socks5ClientConnection> {
public:
    Socks5ClientConnection(boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& endpoint);

    void Start();
    void OnStart(const boost::system::error_code& err);

    void Stop();

    void SendConnectionRequest();
    void OnSendConnectionRequest(const boost::system::error_code& err, size_t bytes);

    void ReceiveConnectionRequestResponse();
    void OnReceiveConnectionRequestResponse(const boost::system::error_code& err, size_t bytes);

    void SendAuthenticationRequest();
    void OnSendAuthenticationRequest(const boost::system::error_code& err, size_t bytes);

    void ReceiveAuthenticationRequestResponse();
    void ReceiveAuthenticationRequestResponse(const boost::system::error_code& err, size_t bytes);

    void Send();
    void OnSend(const boost::system::error_code& err, size_t bytes);

    void Receive();
    void OnReceive(const boost::system::error_code& err, size_t bytes);


private:
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::socket socket_;
    bool is_started_ = false;
    std::array<char, ReadBufferSize> read_buffer_;
    std::array<char, WriteBufferSize> write_buffer_;

};

class Socks5Client : public boost::enable_shared_from_this<Socks5Client> {
public:
    Socks5Client(boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& endpoint);

    void Start();
    void Stop();

private:
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::shared_ptr<Socks5ClientConnection> connection_;

};


#endif //SOCKS5_SOCKS5CLIENT_H
