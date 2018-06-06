//
// Created by Vladimir on 31.05.2018.
//

#pragma once

#ifndef SOCKS5_SOCKS5SERVER_H
#define SOCKS5_SOCKS5SERVER_H

#include "Utilities.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <vector>
#include <array>

const size_t ClientProxyReadBufferSize = 512;
const size_t ClientProxyWriteBufferSize = 512;
const size_t ProxyServerReadBufferSize = 512;
const size_t ProxyServerWriteBufferSize = 512;

class Socks5ServerConnection : public boost::enable_shared_from_this<Socks5ServerConnection> {
public:
    explicit Socks5ServerConnection(boost::asio::io_service& service);

//    Socks5ServerConnection(boost::asio::io_service& service, const boost::asio::ip::udp::endpoint endpoint);

    boost::asio::ip::tcp::socket& GetClientSocket();

    void Start();
//    void OnStart();

    void Stop();

    void ReceiveConnectionRequestHead();
    void OnReceiveConnectionRequestHead(const boost::system::error_code& err, size_t bytes);

    void ReceiveConnectionRequestBody(size_t body_length);
    void OnReceiveConnectionRequestBody(const boost::system::error_code& err, size_t bytes);

    void SendConnectionRequestResponse();
    void OnSendConnectionRequestResponse(const boost::system::error_code& err, size_t bytes);

    void ReceiveAuthenticationRequestUsername();
    void OnReceiveAuthenticationRequestUsername(const boost::system::error_code& err, size_t bytes);

    void ReceiveAuthenticationRequestPassword(std::string username);
    void OnReceiveAuthenticationRequestPassword(const boost::system::error_code& err, size_t bytes);

    void SendAuthenticationRequestResponse();
    void OnSendAuthenticationRequestResponse(const boost::system::error_code& err, size_t bytes);

    void ReceiveFromClient();
    void OnReceiveFromClient(const boost::system::error_code& err, size_t bytes);

    void ReceiveFromServer();
    void OnReceiveFromServer(const boost::system::error_code& err, size_t bytes);

    void SendToClient();
    void OnSendToClient(const boost::system::error_code& err, size_t bytes);

    void SendToServer();
    void OnSendToServer();

private:
    // connection stuff
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::socket from_client_to_proxy_tcp_socket_;
    boost::asio::ip::tcp::socket from_proxy_to_server_tcp_socket_;

    //
    bool is_started_ = false;

    // buffers
    std::array<char, ClientProxyReadBufferSize> client_proxy_read_buffer_;
    std::array<char, ClientProxyWriteBufferSize> client_proxy_write_buffer_;

    std::array<char, ProxyServerReadBufferSize> proxy_server_read_buffer_;
    std::array<char, ProxyServerWriteBufferSize> proxy_server_write_buffer_;

};

class Socks5Server : public boost::enable_shared_from_this<Socks5Server> {
public:
    Socks5Server(boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& enpoint);

    void Start();
    void Stop();

    void Accept();
    void OnAccept(boost::shared_ptr<Socks5ServerConnection> connection, const boost::system::error_code& err);
private:
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_;
};


#endif //SOCKS5_SOCKS5SERVER_H
