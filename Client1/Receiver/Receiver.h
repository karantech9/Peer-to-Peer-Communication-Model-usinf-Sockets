#ifndef RECEIVER_H
#define RECEIVER_H
///////////////////////////////////////////////////////////////////////
// Receiver.h - Receive message from other peer						 //
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
* Receiver receive message from the the other peer and then interpret it 
  based on the interpretation it will enqueue received message into receiving queue.

Public Interface:
=================
public:
Receiver();
Receiver(BlockingQueue<Message>* rQueue_,string path_);
void operator()(Socket& socket_);
void requestForString(Socket& socket_);
void replyForString(Socket& socket_);
void requestForFile(Socket& socket_);
void replyForFile(Socket& socket_);
bool stringHandlingRequest(Socket& si);
bool stringHandlingReply(Socket& si);
bool fileHandlingRequest(Socket& si);
bool fileHandlingReply(Socket& si);
void interpretingReceivingMessage(vector<string> storage, string text);
void interpretingReceivingMessage1(vector<string> storage, string text);
bool receivingBuffer(Socket& socket_,string& receivedContent);
void contentSaving(string text, string contentLength);

Required Files:
===============
Display.h, Receiver.cpp, Sender.h, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_RECEIVER Receiver.cpp

Maintenance History:
====================
ver 1.0 : 10 April 15
- first release
*/
#include "../sockets/sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"

/////////////////////////////////////////////////////////////////////////////
// Receiver class
class Receiver
{
public:
	Receiver(){}
	Receiver(BlockingQueue<Message>* rQueue_,string path_)
	{
		rQueue = rQueue_;
		path = path_;
	}
	void operator()(Socket& socket_);
	void requestForString(Socket& socket_);
	void replyForString(Socket& socket_);
	void requestForFile(Socket& socket_);
	void replyForFile(Socket& socket_);
	bool stringHandlingRequest(Socket& si);
	bool stringHandlingReply(Socket& si);
	bool fileHandlingRequest(Socket& si);
	bool fileHandlingReply(Socket& si);
	void interpretingReceivingMessage(vector<string> storage, string text);
	void interpretingReceivingMessage1(vector<string> storage, string text);
	bool receivingBuffer(Socket& socket_,string& receivedContent);
	void contentSaving(string text, string contentLength);
private:
	BlockingQueue<Message>* rQueue;
	string originalContent;
	string reddfinedContent;
	string path;
};
#endif