#pragma once
#include"common.h"
#include<boost/asio.hpp>
namespace seraphim {
	using namespace boost::asio;
	class SocksSession : public std::enable_shared_from_this<SocksSession>{
	public:
		class Delegate {
			virtual void on_blast(shared_ptr<SocksSession> session)=0;
		};
	public:
		SocksSession(socket&& sock,endpoint dst_end,SocksSession::Delegate* delegate):
			l_sock_(std::move(sock)),
			r_sock_(sock.get_io_service()),
			delegate_(delegate)
		{
			std::cout << "dst_end=" << dst_end << std::endl;
			r_sock_.open(boost::asio::ip::tcp::v4());
			r_sock_.bind(dst_end);
		}
		SocksSession(socket&& sock, socket&& dst,SocksSession::Delegate* delegate) :
			l_sock_(std::move(sock)), 
			r_sock_(std::move(dst)) ,
			delegate_(delegate)
		{
		
		}

		endpoint get_ident() {
			return l_sock_.remote_endpoint();
		}


		virtual ~SocksSession() {
		}
		void start();
	private:
		void traffic(shared_ptr<SocksSession> self);
		void blast(const error_code& ec);
		void handshake_type(shared_ptr<SocksSession> self, const error_code& ec, size_t len);
		void handshake_ipv4(shared_ptr<SocksSession>self, const error_code& ec, size_t len);
		void handshake_host(shared_ptr<SocksSession> self,const error_code& ec, size_t len);
		void handshake_resolve(shared_ptr<SocksSession> self,shared_ptr<ip::tcp::resolver> resolver, const error_code& ec, ip::tcp::resolver::iterator itr);
		void handshake_port(shared_ptr<SocksSession>self, const error_code& ec, size_t len);

		// ll==left_recv rs==right_send 
		void lr_handler(shared_ptr<SocksSession> self,const error_code& ec, size_t size);
		void rr_handler(shared_ptr<SocksSession> self,const error_code& ec, size_t size);
		void ls_handler(shared_ptr<SocksSession> self,const error_code& ec, size_t size);
		void rs_handler(shared_ptr<SocksSession> self,const error_code& ec, size_t size);
	private:
		shared_ptr<Delegate> delegate_;
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
	};
};
