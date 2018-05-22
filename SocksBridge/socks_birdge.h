#pragma once
#include"common.h"
#include<boost/system/error_code.hpp>
#include<boost/asio/ip/tcp.hpp>
namespace seraphim {
	using namespace boost::asio;
	class SocksBirdge {
	public:
		SocksBirdge(const string& form_ip, unsigned short form_port, const string& to_ip, const unsigned to_port=0);
		SocksBirdge(endpoint from, endpoint dst);
		void start_forever(unsigned short work_count);
	private:
		io_service  ios_;
		boost::asio::ip::tcp::acceptor acceptor_{ios_};
		endpoint from_;
		endpoint to_;
	private:
		void accetp_handler(const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock);
		void new_session(boost::asio::ip::tcp::socket&& sock);

		
	};

};
