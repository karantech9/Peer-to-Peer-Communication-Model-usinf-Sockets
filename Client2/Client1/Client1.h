#ifndef CLIENT1_H
#define CLIENT1_H
///////////////////////////////////////////////////////////////////////
// Client.h - participate as client in message passing communication //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron 17, Windows 8                          //
// Application: Demonstration for CSE 687, Project #3, Spring 2015   //
// Author:      Karankumar Patel, Syracuse University                //
//              (315) 751-5637, khpatel@syr.edu                      //
///////////////////////////////////////////////////////////////////////

/*
* Module Operations :
*== == == == == == == == ==
* when server is created it will listen request or reply on particular
  port and ip. after that it will create message and send request to other 
  peer. at the end it will also get the appropiate reply from the other peer. 

Public Interface:
=================
public:
Client1();
Client1(string sourceIP, int sourcePort, string destIP, int destPort,string filePath);

Required Files:
===============
Display.h, Client.cpp, Sender.h, Receiver.h, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_CLIENT Client.cpp

Maintenance History:
====================
ver 1.0 : 10 April 15
- first release
*/

#include "../Message/Message.h"
#include <iostream>
#include <sstream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Client class
class Client1
{
public:
	Client1(){}
	Client1(string sourceIP, int sourcePort, string destIP, int destPort, string filePath);
};
#endif