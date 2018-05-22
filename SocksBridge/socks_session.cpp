#include "socks_session.h"
namespace seraphim {


	void SocksSession::start()
	{

		self_ = shared_from_this();
		auto self = shared_from_this();
		l_sock_.async_read_some(buffer(rbuf_, 257), std::bind(&SocksSession::handshake_type, this, std::placeholders::_1, std::placeholders::_2));
	}

	void SocksSession::blast(const error_code& ec)
	{
		l_sock_.close();
		r_sock_.close();
		self_ = nullptr;
	}

	void SocksSession::handshake_type(const error_code& ec, size_t len)
	{
		if (ec) {
			blast(ec);
		}
		else {
			auto self = shared_from_this();
			shared_array<uint8_t> sbuf(new uint8_t[2]);
			ByteOStream o(sbuf.get(), 2);
			o.putN<uint16_t>(0x0500);
			l_sock_.async_send(buffer(sbuf.get(), 2), [sbuf, this](const error_code& ec, size_t size) {
				if (ec) {
					blast(ec);
				}
			});
			async_read(l_sock_, buffer(rbuf_, 4), [this, self](const error_code& ec, size_t len) {
				if (ec) {
					blast(ec);
				}
				else {
					do {
						shared_array<uint8_t> request_buf(new uint8_t[1024]);
						ByteIStream iBuf(rbuf_, len);
						uint8_t ver = iBuf.getN<uint8_t>();
						uint8_t cmd = iBuf.getN<uint8_t>();
						uint8_t rsv = iBuf.getN<uint8_t>();
						atype_ = iBuf.getN<uint8_t>();
						std::stringstream ss;
						if (cmd != 1) {
							error_code cmd_ec;
							blast(cmd_ec);
							break;
						}
						if (atype_ == 1) {
							l_sock_.async_receive(buffer(rbuf_, 4), std::bind(&SocksSession::handshake_ipv4, this, std::placeholders::_1, std::placeholders::_2));
						}
						else if (atype_ == 3) {
							l_sock_.async_receive(buffer(rbuf_, 1), std::bind(&SocksSession::handshake_host, this, std::placeholders::_1, std::placeholders::_2));
						}
						else {
							error_code atype_err;
							blast(atype_err);
							break;
						}
					} while (false);
				}
			});

		}
	}

	void SocksSession::handshake_ipv4(const error_code& ec, size_t len)
	{
		if (ec) {
			blast(ec);
		}
		else {
			if (atype_ != 1) {
				//LOG_ERROR << this << "{at handshake_ipv4 atype!=4" << atype << "}";
			}
			ByteIStream  iBuf(rbuf_, len);
			dst_ip_ = iBuf.getN<uint32_t>();
			l_sock_.async_receive(buffer(rbuf_, 2), std::bind(&SocksSession::handshake_port, this, std::placeholders::_1, std::placeholders::_2));
		}


	}

	void SocksSession::handshake_host(const error_code& ec, size_t len)
	{
		//LOG_DEBUG << this << "{handshake_host}";
		if (ec) {
			blast(ec);
		}
		else {
			if (atype_ != 3) {
				//LOG_ERROR << this << "{handshake_host atype!=3" << atype << "}";

			}
			ByteIStream iBuf(rbuf_, len);
			uint8_t len = iBuf.getN<uint8_t>();
			iBuf.reset();
			l_sock_.async_receive(buffer(rbuf_, len), [this](const error_code& ec, size_t len) {
				if (ec) {
					blast(ec);
				}
				else {
					char* szbuf = reinterpret_cast<char*>(rbuf_);
					dst_host_ = string(szbuf, szbuf + len);
					ip::tcp::resolver::query q(dst_host_, "http");
					shared_ptr<ip::tcp::resolver> resolver(new ip::tcp::resolver(l_sock_.get_io_service()));
					resolver->async_resolve(q, std::bind(&SocksSession::handshake_resolve, this, resolver, std::placeholders::_1, std::placeholders::_2));
				}
			});
		}


	}

	void SocksSession::handshake_resolve(shared_ptr<ip::tcp::resolver> resolver, const error_code& ec, ip::tcp::resolver::iterator itr)
	{

		//LOG_DEBUG << this << "{handshake_resolve}";
		if (ec) {
			blast(ec);
		}
		else {
			ip::tcp::resolver::iterator end;

			if (itr == end) {
				error_code resolver_error;
				blast(resolver_error);
				return;
			}
			for (; itr != end; itr++) {
				ip::tcp::endpoint  point = *itr;
				if (point.address().is_v4()) {
					dst_ip_ = point.address().to_v4().to_uint();
					break;
				}
			}
			if (dst_ip_ == 0) {
				error_code not_v4_error;
				blast(not_v4_error);
				return;
			}
			l_sock_.async_receive(buffer(rbuf_, 2), std::bind(&SocksSession::handshake_port, this, std::placeholders::_1, std::placeholders::_2));
		}

	}

	void SocksSession::handshake_port(const error_code& ec, size_t len)
	{
		//LOG_DEBUG << this << "{handshake_port}";
		if (ec) {
			blast(ec);
		}
		else {
			ByteIStream  iBuf(rbuf_, len);
			dst_port_ = iBuf.getN<uint16_t>();
		}
		ip::address_v4(dst_ip_);
		dst = endpoint(ip::address_v4(dst_ip_), dst_port_);
		r_sock_.async_connect(dst , [this](const error_code& ec) {
			if (ec) {
				blast(ec);
			}
			else {
				traffic();
			}
		});

	}

	void SocksSession::traffic()
	{
		l_sock_.async_receive(boost::asio::buffer(lbuf_, max_buf), bind(&SocksSession::lr_handler,this,std::placeholders::_1,std::placeholders::_2));
		r_sock_.async_receive(boost::asio::buffer(rbuf_, max_buf), bind(&SocksSession::rr_handler,this,std::placeholders::_1,std::placeholders::_2));
	}

	void SocksSession::lr_handler(const error_code& ec, size_t size)
	{

	}

	void SocksSession::rr_handler(const error_code& ec, size_t size)
	{

	}

	void SocksSession::ls_handler(const error_code& ec, size_t size)
	{

	}

	void SocksSession::rs_handler(const error_code& ec, size_t size)
	{

	}

};


