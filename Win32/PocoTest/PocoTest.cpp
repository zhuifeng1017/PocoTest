// PocoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CSimpleSocket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	CSimpleSocket simple;
	simple.test();

	cin.get();

	return 0;
}

