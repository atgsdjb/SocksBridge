#pragma once
#include"common.h"
#include"socks_session.h"
#include<boost/system/error_code.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<map>
namespace seraphim {
	using namespace boost::asio;
	class SocksBirdge :public std::enable_shared_from_this<SocksBirdge>, SocksSession::Delegate {
	public:
		SocksBirdge(const string& form_ip, unsigned short form_port, const string& to_ip, const unsigned to_port=0);
		SocksBirdge(endpoint from, endpoint dst);
		void start_forever(unsigned short work_count);
	private:
		io_service  ios_;
		boost::asio::ip::tcp::acceptor acceptor_{ios_};
		endpoint from_;
		endpoint to_;
		std::map < endpoint, shared_ptr<SocksSession>> mSession;
		virtual void on_blast(shared_ptr<SocksSession> session) override;

	private:
		void accetp_handler(const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock);
		void new_session(boost::asio::ip::tcp::socket&& sock);

		
	};

};
