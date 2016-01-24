///////////////////////////////////////////////////////////////////////
// Server1.cpp - participate as server in message passing			 //
//				 communication										 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron 17, Windows 8                          //
// Application: Demonstration for CSE 687, Project #3, Spring 2015   //
// Author:      Karankumar Patel, Syracuse University                //
//              (315) 751-5637, khpatel@syr.edu                      //
///////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Receiver/Receiver.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Sender/Sender.h"
#include "../Display/Display.h"
#include "Server1.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace ApplicationHelpers;
using namespace Display;

//----< Dequeue message from the receiving queue >-----------------
void Server1::getQueueMessage(BlockingQueue<Message>* rQueue, Sender* sd)
{
	try
	{
		do{
			Message msg = rQueue->deQ();
			sd->enQFromReceivingQueue(msg);
		} while (true);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< initialize server instance >-----------------
Server1::Server1(string sourceIP, int sourcePort,string path)
{
	title("Testing Socket Server", '=');
	displayPeerInformation("Server", sourceIP, sourcePort);
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(sourcePort, Socket::IP6);
		BlockingQueue<Message> rQueue; //receiving queue
		BlockingQueue<Message> sQueue; //sending queue
		Receiver rc(&rQueue,path);
		Sender sd(&sQueue);
		sl.start(rc);
		thread rThread = std::thread([&, this]{this->getQueueMessage(&rQueue, &sd); });
		std::thread sThread(&Sender::getQueueMessage, Sender(&sQueue));
		sThread.join();
		rThread.join();

		Verbose::show("press key to exit: ", always);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_SERVER1
int main()
{
	Server1 sr("localhost", 2000, "..\\TESTFOLDER2\\");
}
#endif