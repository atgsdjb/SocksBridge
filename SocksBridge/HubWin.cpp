// HubWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<boost/asio.hpp>
#include<boost/program_options.hpp>
#include<iostream>
#include<string>
#include"socks_birdge.h"
using namespace seraphim;
using std::string;

int main(int argc ,char** argv)
{
	string in;
	std::cin >> in;
	std::cout << argv[0]<<in << std::endl;
    return 0;
}

