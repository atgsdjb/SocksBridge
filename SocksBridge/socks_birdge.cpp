#include"common.h"
#include "socks_birdge.h"
#include"socks_session.h"
#include<iostream>
#include<thread>
#include<functional>
using std::thread;
namespace seraphim {
	using namespace boost::asio;
	SocksBirdge::SocksBirdge(const string& from_ip, unsigned short from_port, const string& to_ip, const unsigned to_port)
	{
		from_ = endpoint(ip::address_v4::from_string(from_ip), from_port);
		to_ = endpoint(ip::address_v4::from_string(to_ip), to_port);
	}

	SocksBirdge::SocksBirdge(endpoint from, endpoint dst) :from_(from), to_(dst)
	{

	}

	void SocksBirdge::start_forever(unsigned short work_count)
	{
		std::cout << "from" << from_ << std::endl;
		std::cout << "to" << to_ << std::endl;
		for (int i = 0; i < work_count; i++) {
			thread t([this] {
				boost::asio::io_service::work work(ios_);
				ios_.run();
			});
			t.detach();
		}
		error_code ec;
		acceptor_.open(boost::asio::ip::tcp::v4());
		acceptor_.bind(from_, ec);
		if (ec) {
			std::cout << ec.message() << std::endl;
		}
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.listen();

		acceptor_.async_accept(ios_, std::bind(&SocksBirdge::accetp_handler, this, std::placeholders::_1, std::placeholders::_2));
	}

	void SocksBirdge::on_blast(shared_ptr<SocksSession> session)
	{
		auto itr = mSession.find(session->get_ident());
		if (itr != mSession.end()) {
			mSession.erase(itr);
		}
	}

	void SocksBirdge::accetp_handler(const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock)
	{
		if (ec) {
			std::cout << ec.message() << std::endl;
		}
		else {
			new_session(std::move(sock));
			acceptor_.async_accept(ios_, std::bind(&SocksBirdge::accetp_handler, this, std::placeholders::_1, std::placeholders::_2));
		}

	}

	void SocksBirdge::new_session(boost::asio::ip::tcp::socket&& sock)
	{
		endpoint remote_endpoint = sock.remote_endpoint();
		shared_ptr<SocksSession> session(new SocksSession(std::move(sock), to_, this));
		session->start();
		auto rst = mSession.insert(std::make_pair(remote_endpoint, session));
		if (!std::get<1>(rst)) {
			std::cout << "insert session error" << std::endl;
		}
	}

};
