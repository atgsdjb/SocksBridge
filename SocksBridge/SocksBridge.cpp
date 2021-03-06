// HubWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<boost/asio.hpp>
#include<boost/program_options.hpp>
#include<iostream>
#include<string>

#include"common.h"
#include"socks_birdge.h"
using namespace seraphim;
using std::string;

int main(int argc, char** argv)
{
	auto *from_ip = argv[1];
	unsigned short  from_port = boost::lexical_cast<uint16_t>(argv[2]);
	endpoint  lpoint = endpoint(ip::address_v4::from_string(argv[1]), 3025);
	endpoint  rpoint = endpoint(ip::address_v4::from_string(argv[3]), boost::lexical_cast<uint16_t>(argv[4]));
	shared_ptr<SocksBirdge> birdge(new SocksBirdge(lpoint,rpoint));
	birdge->start_forever(4);
	int i;
	for (;;) {
		std::cin >> i;
		if (i == 0) {
			break;
		}
	}
	return 0;
}

