//
//  CSimpleSocket.cpp
//  PocoTest
//
//  Created by uistrong on 13-8-10.
//  Copyright (c) 2013年 uistrong. All rights reserved.
//

#include "CSimpleSocket.h"

#include "Poco/Unicode.h"
#include "Poco/Net/DNS.h"
//#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocket.h"

void CSimpleSocket::test(){
    using namespace Poco;
    using namespace Poco::Net;
    
    StreamSocket sk;
    BOOL connected = NO;
    try{
        sk.connect(SocketAddress("s1.goyou.cn", 80), Timespan(6, 0));
        connected = YES;
    }catch(Poco::Exception& exp){
        std::cout <<"code:" << exp.what() << "-msg:" << exp.message() << std::endl;
    }
    
    if (connected) {
        try{
            UInt8 buffer[2048] = "GET / HTTP/1.0\r\nUser-Agent: Mozilla/5.0\r\n\r\n";
            int nSendLen = strlen((char*)buffer);
            int nHasSendLen = 0;
            while (nHasSendLen != nSendLen) {
                int nLen = sk.sendBytes(buffer+nHasSendLen, nSendLen-nHasSendLen);
                nHasSendLen += nLen;
            }
            
            int nRecvLen = 0;
            do {
                Socket::SocketList readList, writeList, expList;
                readList.push_back(sk);
                Socket::select(readList, writeList, expList, Timespan(3, 0));
                
                if (readList.size()) {
                    nRecvLen = sk.receiveBytes(buffer, 1024);
                    std::cout << nSendLen << " -- " << nRecvLen << std::endl;
                    buffer[nRecvLen] = 0;
                    std::cout << buffer << std::endl;
                }
            } while (nRecvLen!=0);
            
            sk.close();
        }catch(Poco::Exception& exp){
            std::cout <<"code:" << exp.code() << "-msg:" << exp.message() << std::endl;
        }
    }
}