//
// Created by Vladimir on 31.05.2018.
//

#include "../Src/Socks5Server.h"
#include <glog/logging.h>

const size_t port = 1080;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = "../../Log/ServerLog";
    boost::asio::io_service service;
    auto server = boost::make_shared<Socks5Server>(service,
                        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    server->Start();
    service.run();
    return 0;
}
