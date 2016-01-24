//////////////////////////////////////////////////////////////////////////
// Executive.cpp - Manage all Packages of Message Passing Communication///
// ver 1.0                                                              //
// Language:    C++, Visual Studio 13.0						            //
// Platform:    Dell Inspiron 17, Windows 8                             //
// Application: Demonstration for CSE 687, Project #3, Spring 2015      //
// Author:      Karankumar Patel, Syracuse University                   //
//              (315) 751-5637, khpatel@syr.edu                         //
//////////////////////////////////////////////////////////////////////////

#include"../Client1/Client1.h"
#include "../Server1/Server1.h"
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include "Executive.h"
#include <iostream>
#include <thread>
using namespace std;

//----<the program starts from this point>------------------------------
#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	string sourceIP = argv[1];
	int sourcePort = stoi(argv[2]);
	string destIP = argv[3];
	int destPort = stoi(argv[4]);
	string fs = FileSystem::Path::fileSpec(argv[5], argv[6]);
	string filepath = FileSystem::Path::getFullFileSpec(fs);

	string s = "Client2 - Port: " + to_string(sourcePort);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	SetConsoleTitle(r.c_str());

	Client1 cl(sourceIP, sourcePort, destIP, destPort, filepath);//create client instance
}
#endif