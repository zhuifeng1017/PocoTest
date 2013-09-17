#include "Socks5Client.h"
#include "Poco/Net/Socket.h"

CSocks5Client::CSocks5Client(const char *ipAddr,  unsigned short port, const char *digest)
{
	_ipAddr = ipAddr;
	_port = port;
	_digest = digest;
}

CSocks5Client::~CSocks5Client(void)
{
}

bool CSocks5Client::GetSocket(Poco::Net::StreamSocket &sk){
	using namespace Poco;
	using namespace Poco::Net;

	try{
		sk.connect(SocketAddress(_ipAddr.c_str(), _port), Timespan(6, 0));
	}catch(Poco::Exception& exp){
		std::cout << exp.displayText() << std::endl;
		return false;
	}

	if (!Establish(sk))
	{
		sk.close();
		return false;
	}
	return true;
}

bool CSocks5Client::Establish(Poco::Net::StreamSocket &sk){
	using namespace Poco;
	using namespace Poco::Net;
	UInt8 cmd[3];
	cmd[0] = (UInt8) 0x05; // protocol version 5
	cmd[1] = (UInt8) 0x01; // number of authentication methods supported
	cmd[2] = (UInt8) 0x00; // authentication method: no-authentication required\

	int nHasSendLen = sk.sendBytes(cmd, 3);
	if (nHasSendLen != 3)
	{
		return false;
	}

	// 接收回应
	Socket::SocketList readList, writeList, expList;
	readList.push_back(sk);
	int rc = Socket::select(readList, writeList, expList, Timespan(3, 0));

	if (rc <=0 || readList.size()<=0)
	{
		return false;
	}
	
	UInt8 response[2];
	int nRecvLen = sk.receiveBytes(response, 2);
	if (nRecvLen != 2)
	{
		return false;
	}

	// check if server responded with correct version and no-authentication method
	if (response[0] != (UInt8) 0x05 || response[1] != (UInt8) 0x00) {
		return false;
	}

	UInt8 requestBuff[128];
	UInt32 requestBuffLen = 128;
	// request SOCKS5 connection with given address/digest
	if (!CreateSocks5ConnectRequest(requestBuff, requestBuffLen))
	{
		return false;
	}

	nHasSendLen = sk.sendBytes(requestBuff, requestBuffLen);
	if (nHasSendLen != requestBuffLen)
	{
		return false;
	}

	// receive connection response
	readList.clear();	
	writeList.clear(); 
	expList.clear();
	readList.push_back(sk);
	rc = Socket::select(readList, writeList, expList, Timespan(3, 0));
	if (rc <=0 || readList.size() <=0)
	{
		return false;
	}

	UInt8 responseBuff[128];
	UInt32 responseBuffLen;
	nRecvLen = sk.receiveBytes(responseBuff, 5);
	if (nRecvLen != 5)
	{
		return false;
	}
	int addressLength = responseBuff[4];
	nRecvLen = sk.receiveBytes(&responseBuff[5], addressLength+2);
	if (nRecvLen != (addressLength+2))
	{
		return false;
	}
	responseBuffLen = 5 + addressLength +  2;

	// verify response
	requestBuff[1] = (UInt8) 0x00; // set expected return status to 0
	if (requestBuffLen != responseBuffLen)
	{
		return false;
	}
	return (memcmp(requestBuff, responseBuff, requestBuffLen)==0);
}

bool CSocks5Client::CreateSocks5ConnectRequest(Poco::UInt8 *requestBuff,  Poco::UInt32 &buffLen){
	using namespace Poco;
	using namespace Poco::Net;

	UInt32 dataLen = 7 + _digest.length();
	if (buffLen < dataLen)
	{
		return false;
	}

	requestBuff[0] = (UInt8) 0x05; // version (SOCKS5)
	requestBuff[1] = (UInt8) 0x01; // command (1 - connect)
	requestBuff[2] = (UInt8) 0x00; // reserved byte (always 0)
	requestBuff[3] = (UInt8) 0x03; // address type (3 - domain name)
	requestBuff[4] = (UInt8) _digest.length(); // address length
	memcpy(&requestBuff[5], _digest.c_str(), _digest.length());
	requestBuff[dataLen - 2] = (UInt8) 0; // address port (2 bytes always 0)
	requestBuff[dataLen - 1] = (UInt8) 0;
	buffLen = dataLen;
	return true;
}


