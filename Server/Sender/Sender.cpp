///////////////////////////////////////////////////////////////////////
// Sender.cpp - Send message to other peer							 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron 17, Windows 8                          //
// Application: Demonstration for CSE 687, Project #3, Spring 2015   //
// Author:      Karankumar Patel, Syracuse University                //
//              (315) 751-5637, khpatel@syr.edu                      //
///////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include"Sender.h"
#include <iostream>
#include <sys/stat.h>

using namespace ApplicationHelpers;
using namespace Display;
using namespace FileSystem;

//----< Dequeue message from the sending queue >-----------------
void Sender::getQueueMessage()
{
	try
	{
		do
		{
			Message msg = sQueue->deQ();
			interpretingSendingMessage(msg);
		} while (true);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}

}

//----< Enqueue message to the sending queue from Receiving queue >-----------------
void Sender::enQFromReceivingQueue(Message msg)
{
	try
	{
		sQueue->enQ(msg);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< interpret sending message >-----------------
void Sender::interpretingSendingMessage(Message& msg)
{
	string command = msg.getCommand();
	map<string, string> att = msg.getAttributes();
	string destIP = att["destIP"];
	int destPort = atoi(att["destPort"].c_str());
	try{
		displayLine('=');
		Verbose v(true);
		SocketSystem ss;
		SocketConnecter si;
		BlockingQueue<Message> sQueue;
		while (!si.connect(destIP, destPort))
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		if (command == "STRING_HANDLING_REQUEST")
		{
			displayString("Demonstrating: Sending string to one peer to other peer");
			displayLine(' ');
			stringHandlingRequest(si, msg);
		}
		if (command == "STRING_HANDLING_REPLY")
		{
			displayString("Demonstrating: Reply from one peer to other peer - String Received");
			displayLine(' ');
			stringHandlingReply(si, msg);
		}
		if (command == "FILE_HANDLING_REQUEST")
		{
			displayString("Demonstrating: Sending file to one peer to other peer");
			displayLine(' ');
			fileHandlingRequest(si, msg);
		}
		if (command == "FILE_HANDLING_REPLY")
		{
			displayString("Demonstrating: Reply from one peer to other peer - File Received");
			displayLine(' ');
			fileHandlingReply(si, msg);
		}
		si.shutDownSend();
	}
	catch (std::exception& ex){
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
	displayLine('-');
}

//----< sending string handling reply to other peer >--------
void Sender::stringHandlingReply(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("body:" + att["body"]);
	displayString("Peer(Server):" + att["sourcePort"] + " Sending String received acknowledgement to Peer(Client):" + att["destPort"]);
}

//----< sending file handling reply to other peer >--------
void Sender::fileHandlingReply(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("fileName:" + att["fileName"]);
	si.sendString("body:" + att["body"]);
	displayString("Peer(Server):" + att["sourcePort"] + " Sending " + Path::getName(att["fileName"], true) + " received acknowledgement to Peer(Client):" + att["destPort"]);
}

//----< sending string handling request to other peer >--------
void Sender::stringHandlingRequest(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("contentLength:" + att["contentLength"]);
	si.sendString("\n");

	const int BufLen = 1024;
	Socket::byte buffer[BufLen];
	std::string text = att["body"];
	for (size_t i = 0; i < BufLen; ++i)
	{
		if (i < text.size())
			buffer[i] = text[i];
		else
			buffer[i] = '#';
	}
	buffer[BufLen - 1] = '\0';
	si.send(BufLen, buffer);
	
	displayString("command:" + msg.getCommand());
	Verbose::show("\n", always);
	displayString("Peer(Server):" + att["sourcePort"] + " Sending one buffer of string to other Peer(Client):" + att["destPort"]);
	Verbose::show("\n", always);
	Verbose::show("client sending buffer: " + std::string(buffer), always);
}

//----< sending file handling request to other peer >--------
void Sender::fileHandlingRequest(Socket& si, Message& msg)
{
	map<string, string> att = msg.getAttributes();
	displayString("command:" + msg.getCommand());
	displayString("\nClient sending file located at->" + att["fileName"] + "\n\n");

	if (!findFile(att["fileName"])){
		Verbose::show("file Not Found!!", always);
		return;
	}
	const size_t bufLen = 1024;
	File::byte buffer[bufLen];
	File bufferIn(att["fileName"]);
	bufferIn.open(File::in, File::binary);

	while (true)
	{
		int resultSize = bufferIn.getBuffer(bufLen, buffer);
		att["contentLength"] = std::to_string(resultSize);
		string temp1(buffer, resultSize);

		if (resultSize < 1024)
		{
			for (int i = temp1.size(); i < 1024; i++)
				buffer[i] = '#';
			resultSize = 1024;
		}
		si.sendString("command:" + msg.getCommand());
		si.sendString("sourceIP:" + att["sourceIP"]);
		si.sendString("sourcePort:" + att["sourcePort"]);
		si.sendString("destIP:" + att["destIP"]);
		si.sendString("destPort:" + att["destPort"]);
		si.sendString("fileName:" + att["fileName"]);
		si.sendString("contentLength:" + att["contentLength"]);
		si.sendString("\n");
		si.send(resultSize, buffer);
		displayString("Peer(Server):" + att["sourcePort"] + " Sending one buffer of " + Path::getName(att["fileName"], true) + " to other Peer(Client):" + att["destPort"]);
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			bufferIn.close();
			displayString("----Client has sent whole " + Path::getName(att["fileName"], true) + " to server----");
			break;
		}
	}
}

//----< check file is located on specified path >--------
bool Sender::findFile(std::string name)
{
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}

	/*std::vector<std::string> testFiles = FileSystem::Directory::getFiles("../TESTFOLDER1");
	for (auto file : testFiles)
	{
		if (name.compare(file) == 0)
			return true;
	}
	return false;*/
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_SENDER
int main()
{
	title("Testing Sender", '=');
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(1000, Socket::IP6);
		BlockingQueue<Message> rQueue;
		BlockingQueue<Message> sQueue;
		Sender sd(&sQueue);

		string test = "Test Message";
		string fileName = "test1.txt";
		Message msg("STRING_HANDLING_REQUEST");
		msg.buildMessage(msg, 1000, 2000, test, test.size());

		displayMessage(msg, "Constructing");
		Message msg2("FILE_HANDLING_REQUEST");
		msg.buildMessage(msg2, 1000, 2000, fileName);
		sQueue.enQ(msg);
		sQueue.enQ(msg2);

		std::thread sThread(&Sender::getQueueMessage, Sender(&sQueue));
		sThread.join();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif
