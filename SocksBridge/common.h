#pragma once
#include<string>
#include<memory>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/ip/address.hpp>
#include<boost/asio/io_service.hpp>
namespace seraphim {
using std::string;
using std::shared_ptr;
using endpoint = boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> ;
using socket = boost::asio::ip::tcp::socket;
};
