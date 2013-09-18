//
//  MZAppDelegate.m
//  PocoTest
//
//  Created by uistrong on 13-5-27.
//  Copyright (c) 2013年 uistrong. All rights reserved.
//

#import "MZAppDelegate.h"
#import "MZViewController.h"

#include <iostream>
#include "CSimpleSocket.h"
#include "Socks5Client.h"
#include "SendRecvFile.h"


@implementation MZAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    
//    CSimpleSocket simple;
//    simple.test();

    using namespace std;
	using namespace Poco;
	using namespace Poco::Net;
    
    //03de6c570bfe24bfc328ccd7ca46b76eadaf4334
	//SHA1Engine engine;
	//engine.update("abcde");
	//cout << "_encryptDigest : " <<  SHA1Engine::digestToHex(engine.digest()) << endl;
    
	CSocks5Client skClient("192.168.108.213", 7778, "abcde");
	StreamSocket sk;
	bool ret = skClient.GetSocket(sk);
	if (!ret)
	{
		goto tagend;
	}
    
	if (1)
	{
		cout << "begin recv data" << endl;
		CSendRecvFile::recvFile(sk);
	}else
	{
		cout << "begin send data" << endl;
		//const char *filePath = "D:\\„Î¥®µÿ«¯.mimg";
		const char *filePath = "e:\\20130917085141.bmp";
		CSendRecvFile::sendFile(sk, filePath);
	}
	sk.close();
    
tagend:
	cout << "press any key to exit" << endl;
    
    
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
