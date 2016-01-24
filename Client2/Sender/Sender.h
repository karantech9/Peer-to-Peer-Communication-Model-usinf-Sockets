#ifndef SENDER_H
#define SENDER_H
///////////////////////////////////////////////////////////////////////
// Sender.h - Send message to other peer							 //
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
* Sender first deqeue message from the sendind queue and iterpret it to 
  find out destination ip and port. based on the request it will send the 
  appropiate message to other peer.

Public Interface:
=================
public:
Sender();
Sender(BlockingQueue<Message>* sQueue_);
void enQFromReceivingQueue(Message msg);
void getQueueMessage();
void interpretingSendingMessage(Message& msg);
void stringHandlingRequest(Socket& si, Message& msg);
void stringHandlingReply(Socket& si, Message& msg);
void fileHandlingRequest(Socket& si, Message& msg);
void fileHandlingReply(Socket& si, Message& msg);
bool findFile(std::string name);

Required Files:
===============
Display.h, Sender.cpp, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_SENDER Sender.cpp

Maintenance History:
====================
ver 1.0 : 10 April 15
- first release
*/

#include "../Message/Message.h"
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"

/////////////////////////////////////////////////////////////////////////////
// Sender class
class Sender
{
public:
	Sender(){}
	Sender(BlockingQueue<Message>* sQueue_)
	{
		sQueue = sQueue_;
	}
	void enQFromReceivingQueue(Message msg);
	void getQueueMessage();
	void interpretingSendingMessage(Message& msg);
	void stringHandlingRequest(Socket& si, Message& msg);
	void stringHandlingReply(Socket& si, Message& msg);
	void fileHandlingRequest(Socket& si, Message& msg);
	void fileHandlingReply(Socket& si, Message& msg);
	bool findFile(std::string name);

private:
	BlockingQueue<Message>* sQueue;
};
#endif