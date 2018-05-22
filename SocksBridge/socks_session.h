#pragma once
#include"common.h"
#include<boost/asio.hpp>
namespace seraphim {
	using namespace boost::asio;
	class SocksSession : public std::enable_shared_from_this<SocksSession>{
	public:
		SocksSession(socket&& sock,endpoint dst_end):sock_(std::move(sock)),dst_sock_(sock.get_io_service())
		{
			dst_sock_.bind(dst_end);
		}
		SocksSession(socket&& sock, socket&& dst) :sock_(std::move(sock)), dst_sock_(std::move(dst)) {

		}
		virtual ~SocksSession() {
		}
		void traffic();
		void band();

	private:
		ip::tcp::socket sock_;
		ip::tcp::socket dst_sock_;
		uint8_t rbuf[4096];
		shared_ptr<SocksSession> self_;


	};


};
