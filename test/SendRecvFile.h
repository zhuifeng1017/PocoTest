#pragma once
#include "Poco/Net/StreamSocket.h"

class CSendRecvFile{
public:
	CSendRecvFile(void);
	~CSendRecvFile(void);

	static void sendFile(Poco::Net::StreamSocket &sk);
	static bool recvFile(Poco::Net::StreamSocket &sk);

};