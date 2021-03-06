// p_network.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\..\units\network\address\u_address.h"
#include "..\..\units\network\u_network.h"

int main()
{
	network::library library;

	network::address::host host_0;
	network::address::host host_1( {1, 2, 3, 4} );
	const auto s = host_1.to_string();
	network::address::host host_2{};
	network::address::host host_3{host_1};

	auto x = host_0.octet(1);
	auto &x_ref = host_0.octet(1);
	host_0.octet(1) = 23;

	host_1 = { 11, 12, 13, 14 };
	host_1 = {11, 12, 13, 14};
	//host_1 = network::address::host::octets({ 11, 12, 13, 14, 15 });
	host_1 = {21, 22, 23};
	host_1 = { 21, 22 };
	host_1 = { 21 };
	//host_1 = {};
	host_1 = host_2;
}

