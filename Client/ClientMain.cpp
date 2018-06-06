//
// Created by Vladimir on 31.05.2018.
//

#include "../Src/Socks5Client.h"
#include <glog/logging.h>

// const char ip_address[] = "192.168.1.52";

const size_t port = 1080;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = "../../Log/ClientLog";
    boost::asio::io_service service;
    auto client = boost::make_shared<Socks5Client>(service,
                        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    client->Start();
    service.run();
    return 0;
}
