#pragma once
#include <string>
#include "Poco/Net/Net.h"
#include "Poco/Net/StreamSocket.h"

class CSocks5Client
{
public:
	CSocks5Client(const char *ipAddr,  unsigned short port, const char *digest);
	~CSocks5Client(void);
	
	bool GetSocket(Poco::Net::StreamSocket &sk);

private:
	bool Establish(Poco::Net::StreamSocket &sk);
	bool CreateSocks5ConnectRequest(Poco::UInt8 *requestBuff, Poco::UInt32 &buffLen);

private:
	std::string _ipAddr;
	unsigned short _port;
	std::string _digest;
	std::string _encryptDigest;
};
