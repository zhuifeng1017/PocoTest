//
//  MZAppDelegate.m
//  PocoTest
//
//  Created by uistrong on 13-5-27.
//  Copyright (c) 2013年 uistrong. All rights reserved.
//

#import "MZAppDelegate.h"

#import "MZViewController.h"
#include "Poco/Unicode.h"
#include "Poco/Net/DNS.h"
//#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocket.h"

#include <iostream>

@implementation MZAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
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

    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    self.viewController = [[MZViewController alloc] initWithNibName:@"MZViewController" bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
