#include "SendRecvFile.h"
#include "Poco/Foundation.h"
#include "Poco/Net/Net.h"
#include "Poco/Timespan.h"
#include "poco/Timestamp.h"
#include <iostream>


using namespace Poco;
using namespace Poco::Net;

CSendRecvFile::CSendRecvFile(void)
{
}

CSendRecvFile::~CSendRecvFile(void)
{
}

void CSendRecvFile::sendFile(Poco::Net::StreamSocket &sk, const char* filepath){
	using namespace std;

	UInt8 buffer[8192];
	const int bufferLen = 8192;
	int total = 0;
	for (int i=0; i <1; ++i)
	{
		//const char *filePath = "D:\\ãë´¨µØÇø.mimg";
		//const char *filePath = "e:\\20130917085141.bmp";
		FILE * file = fopen(filepath, "rb");
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
    Poco::UInt8 buffer[8192];
	int total = 0;
	int tempTotal = 0;
	Timestamp start;
	Timespan::TimeDiff totalCost = 0;
	while (1)
	{
		// calc speed
		for (int i=0; i<10; ++i){
			int nRecvLen = sk.receiveBytes(buffer, 8192);
			total += nRecvLen;	
			tempTotal += nRecvLen;
			//cout << "this recv: " << nRecvLen << "- total length: " << total << endl;
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

		Poco::Timespan elapsed = start.elapsed();
		Timespan::TimeDiff cost = elapsed.totalMilliseconds();
		if (cost>500) // more than 500 millisec calc one time
		{
			totalCost += cost;
			int speed = tempTotal/cost;
			cout <<"totalCost: " << totalCost/1000.0f <<  " s-- recv speed:" << speed << " kb/s" << endl;
			tempTotal = 0;
			start.update(); // reset timer count
		}
	}
	return true;
}
