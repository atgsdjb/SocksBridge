#include"common.h"
#include "socks_birdge.h"
#include"socks_session.h"
#include"socks_session.h"
#include<thread>
#include<functional>
using std::thread;
namespace seraphim {
	using namespace boost::asio	;
	SocksBirdge::SocksBirdge(const string& from_ip, unsigned short from_port, const string& to_ip, const unsigned to_port)
	{
		from_ = endpoint(ip::address_v4::from_string(from_ip), from_port);
		to_ = endpoint(ip::address_v4::from_string(to_ip), to_port);
	}

	SocksBirdge::SocksBirdge(endpoint from, endpoint dst):from_(from),to_(dst)
	{

	}

	void SocksBirdge::start_forever(unsigned short work_count)
	{
		for (int i = 0; i < work_count; i++) {
			thread t([this] {
				boost::asio::io_service::work work(ios_);
				ios_.run();
			});
			t.detach();
		}
		acceptor_.bind(from_);
		acceptor_.async_accept(ios_, std::bind(&SocksBirdge::accetp_handler,this, std::placeholders::_1, std::placeholders::_2 ));
	}

	void SocksBirdge::accetp_handler(const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock)
	{
		new_session(std::move(sock));
		acceptor_.async_accept(ios_, std::bind(&SocksBirdge::accetp_handler, this, std::placeholders::_1, std::placeholders::_2));

	}

	void SocksBirdge::new_session(boost::asio::ip::tcp::socket&& sock)
	{
		auto remote_endpoint = sock.remote_endpoint();
		shared_ptr<SocksSession> session = std::make_shared<SocksSession>(std::move(sock), to_);
		session->traffic();
	}

};
