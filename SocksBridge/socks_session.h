#pragma once
#include"common.h"
#include<boost/asio.hpp>
namespace seraphim {
	using namespace boost::asio;
	class SocksSession : public std::enable_shared_from_this<SocksSession>{
	public:
		SocksSession(socket&& sock,endpoint dst_end):l_sock_(std::move(sock)),r_sock_(sock.get_io_service())
		{
			r_sock_.bind(dst_end);
		}
		SocksSession(socket&& sock, socket&& dst) :l_sock_(std::move(sock)), r_sock_(std::move(dst)) {
		}
		virtual ~SocksSession() {
		}
		void start();
	private:
		void traffic();
		void blast(const error_code& ec);
		void handshake_type(const error_code& ec, size_t len);
		void handshake_ipv4(const error_code& ec, size_t len);
		void handshake_host(const error_code& ec, size_t len);
		void handshake_resolve(shared_ptr<ip::tcp::resolver> resolver, const error_code& ec, ip::tcp::resolver::iterator itr);
		void handshake_port(const error_code& ec, size_t len);

		// ll==left_revv rs==right_send 
		void lr_handler(const error_code& ec, size_t size);
		void rr_handler(const error_code& ec, size_t size);
		void ls_handler(const error_code& ec, size_t size);
		void rs_handler(const error_code& ec, size_t size);
	private:
		static constexpr size_t max_buf{4096};
		//left socket
		ip::tcp::socket l_sock_;

		//right socket
		ip::tcp::socket r_sock_;
		

		//dst
		uint8_t atype_{ 0 };
		uint32_t dst_ip_{ 0 };
		string dst_host_{ "" };
		uint16_t dst_port_{ 0 };
		endpoint dst;


		uint8_t rbuf_[4096];
		uint8_t lbuf_[4096];
		shared_ptr<SocksSession> self_;
	};
};
