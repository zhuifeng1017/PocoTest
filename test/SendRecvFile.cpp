#include "SendRecvFile.h"
#include "Poco/Net/Net.h"

using namespace Poco;
using namespace Poco::Net;

CSendRecvFile::CSendRecvFile(void)
{
}

CSendRecvFile::~CSendRecvFile(void)
{
}

void CSendRecvFile::sendFile(Poco::Net::StreamSocket &sk){
	using namespace std;

	UInt8 buffer[8192];
	const int bufferLen = 8192;
	int total = 0;
	for (int i=0; i <1; ++i)
	{
		const char *filePath = "D:\\ãë´¨µØÇø.mimg";
		//const char *filePath = "e:\\20130917085141.bmp";
		FILE * file = fopen(filePath, "rb");
		if (file == NULL)
			return;

		while (!feof(file))
		{
			int count = fread(buffer, 1, bufferLen, file);
			if (count == 0)
			{
				cout << "file read over" << endl;
				break;
			}
			if( ferror( file)) {
				cout << "read file error" << endl;
			}
			// ·¢ËÍ
			int sendCount = sk.sendBytes(buffer, count);
			if (sendCount != count)
			{
				break;
			}
			total += count;
			cout << "read count:" << count <<  "--send total size:" << total << endl;
		}
		fclose(file);
	}
}

bool CSendRecvFile::recvFile(Poco::Net::StreamSocket &sk){
	using namespace std;
	// wait for recv data
	Socket::SocketList readList, writeList, expList;
	readList.push_back(sk);
	int rc = Socket::select(readList, writeList, expList, Timespan(300, 0)); // 5 min
	if (rc <=0 || readList.size()<=0)
	{
		return false;
	}

	// loop recv data
	UINT8 buffer[8192];
	int total = 0;
	while (1)
	{
		int nRecvLen = sk.receiveBytes(buffer, 8192);
		total += nRecvLen;
		cout << "total length: " << total << endl;
		if (nRecvLen <=0)
		{
			return false;
		}

		readList.clear();
		writeList.clear(); 
		expList.clear();
		readList.push_back(sk);
		int rc = Socket::select(readList, writeList, expList, Timespan(30, 0)); // 30 sec
		if (rc <=0 || readList.size()<=0)
		{
			return false;
		}
	}

	return true;
}
