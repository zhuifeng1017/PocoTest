#ifndef __PocoTest__CSendRecvFile__
#define __PocoTest__CSendRecvFile__

#include "Poco/Net/StreamSocket.h"

class CSendRecvFile{
public:
	CSendRecvFile(void);
	virtual ~CSendRecvFile(void);

	static void SendFile(Poco::Net::StreamSocket &sk, const char* filepath);
	static bool RecvFile(Poco::Net::StreamSocket &sk);

};

#endif /* defined(__PocoTest__CSendRecvFile__) */