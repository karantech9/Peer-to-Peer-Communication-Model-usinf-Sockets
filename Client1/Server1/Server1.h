#ifndef SERVER1_H
#define SERVER1_H
///////////////////////////////////////////////////////////////////////
// Server1.h - participate as server in message passing communication//
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
  port and ip. after that once it get message from  the other peer it will 
  interpret it via receiver and send appropiate reply to sending peer via sender

Public Interface:
=================
public:
Server1();
Server1(string ip, int port);
void getQueueMessage(BlockingQueue<Message> rQueue);
void Server1::getQueueMessage(BlockingQueue<Message>* rQueue, Sender* sd);

Required Files:
===============
Display.h, Server1.cpp, Sender.h, Receiver.h, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_SERVER Server.cpp

Maintenance History:
====================
ver 1.0 : 10 April 15
- first release
*/
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../Sender/Sender.h"

/////////////////////////////////////////////////////////////////////////////
// Server class
class Server1
{
public:
	Server1(){}
	Server1(string sourceIP, int sourcePort, string path);
	void getQueueMessage(BlockingQueue<Message> rQueue);
	void Server1::getQueueMessage(BlockingQueue<Message>* rQueue, Sender* sd);
};
#endif