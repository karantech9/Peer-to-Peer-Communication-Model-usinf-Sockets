///////////////////////////////////////////////////////////////////////
// Receiver.h - Receive message from other peer						 //
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
#include "Receiver.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace ApplicationHelpers;
using namespace FileSystem;
using namespace Display;
using namespace std;

//----< Server Receiver Handler thread starts running this function >-----------------
void Receiver::operator()(Socket& socket_)
{
	while (true)
	{
		std::string commandString = socket_.recvString();
		size_t index = commandString.find(":");
		std::string command = commandString.substr(index + 1);
		if (command.size() == 0)
			break;
		displayLine('=');
		Verbose::show("command: " + command);
		if (command == "STRING_HANDLING_REQUEST")
		{
			requestForString(socket_);
			continue; // go back and get another command
		}
		if (command == "STRING_HANDLING_REPLY")
		{
			replyForString(socket_);
			continue;  // go back and get another command
		}
		if (command == "FILE_HANDLING_REQUEST")
		{
			requestForFile(socket_);
			continue; // go back and get another command
		}
		if (command == "FILE_HANDLING_REPLY")
		{
			replyForFile(socket_);
			continue; // get another command
		}
	}
	displayLine('-');
	socket_.shutDown();
	socket_.close();	
}

//----< Check String Handling Request Passed or Failed >-----------------
void Receiver::requestForString(Socket& socket_)
{
	if (stringHandlingRequest(socket_))
		Verbose::show("----String Handling Request test passed----", always);
	else
		Verbose::show("----String Handling Request test failed----", always);
}

//----< Check String Handling Reply Passed or Failed >-----------------
void Receiver::replyForString(Socket& socket_)
{
	if (stringHandlingReply(socket_))
		Verbose::show("----String Handling Reply test passed----", always);
	else
		Verbose::show("----String Handling Reply test failed----", always);
}

//----< Check File Handling Request Passed or Failed >-----------------
void Receiver::requestForFile(Socket& socket_)
{
	if (fileHandlingRequest(socket_))
		Verbose::show("----File Handling Request test passed----", always);
	else
		Verbose::show("----File Handling Request test failed----", always);
}

//----< Check File Handling Reply Passed or Failed >-----------------
void Receiver::replyForFile(Socket& socket_)
{
	if (fileHandlingReply(socket_))
		Verbose::show("----File Handling Reply test passed----", always);
	else
		Verbose::show("----File Handling Reply test failed----", always);
}

//----< Save content received from the other peer >-----------------
void Receiver::contentSaving(string text,string contentLength)
{
	int length = stoi(contentLength);
	originalContent = originalContent + text;
	if (length == 1024)
	{
		for (unsigned int i = 0; i < text.size()-1; i++)
			reddfinedContent = reddfinedContent + text[i];
	}
	else
	{
		for (int i = 0; i < length; i++)
			reddfinedContent = reddfinedContent + text[i];
	}
}

//----< Receive Buffer from the other peer >-----------------
bool Receiver::receivingBuffer(Socket& socket_,string& receivedContent)
{
	const int BufLen = 1024;
	Socket::byte buffer[BufLen];
	bool ok;
	ok = socket_.recv(BufLen, buffer);
	if (socket_ == INVALID_SOCKET || !ok)
		return false;
	for (size_t i = 0; i < BufLen; ++i)
		receivedContent += buffer[i];
	return true;
}

//----< test string handling request >-----------------------------------------------
bool Receiver::stringHandlingRequest(Socket& socket_)
{
	std::vector<std::string> receiverStorage;
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{	
			if (str == "\n")
				break;
			else
				receiverStorage.push_back(str);
		}
		else
			break;
	}
	string receivedContent;
	if (!receivingBuffer(socket_, receivedContent))
		return false;
	displayString("Server receiving one buffer of string from Client");
	interpretingReceivingMessage(receiverStorage, receivedContent);
	return true;
}

//----< interpreting string handling request message >------------------------------
void Receiver::interpretingReceivingMessage(vector<string> storage,string text)
{
	Message msg("STRING_HANDLING_REPLY");
	map<string, string>& attributes = msg.getAttributes();
	for (auto str : storage)
	{
		size_t index = str.find(":");
		std::string attribute = str.substr(0, index);
		std::string value = str.substr(index + 1);
		attributes[attribute] = value;
	}
	contentSaving(text, attributes["contentLength"]);
	if (!(attributes["contentLength"] == "1024"))
	{
		string temp = attributes["sourceIP"];
		attributes["sourceIP"] = attributes["destIP"];
		attributes["destIP"] = temp;
		string temp1 = attributes["sourcePort"];
		attributes["sourcePort"] = attributes["destPort"];
		attributes["destPort"] = temp1;
		attributes["body"] = "string Received Successfully";
		displayString("Server received whole string from Client");
		Verbose::show("\n Received Content  : " + originalContent, always);
		originalContent = "";
		Verbose::show("\n Reveised Content: " + reddfinedContent, always);
		reddfinedContent = "";

		rQueue->enQ(msg);//enQ message after receiving whole string
	}
}

//----< test string handling reply >-----------------------------------------------
bool Receiver::stringHandlingReply(Socket& socket_)
{

	map<string, string> attributes;
	for (int i = 0; i < 5; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			size_t index = str.find(":");
			std::string attribute = str.substr(0, index);
			std::string value = str.substr(index + 1);
			attributes[attribute] = value;
		}
		else
			break;
	}
	displayString("Peer(Client):" + attributes["destPort"] + " Receiving String received acknowledgement from Peer(Server):" + attributes["sourcePort"]);
	//displayString("Client receiving string received acknowledgement from Server");
	return true;
}

//----< test file handling reply >-----------------------------------------------
bool Receiver::fileHandlingReply(Socket& socket_)
{
	map<string, string> attributes;
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			size_t index = str.find(":");
			std::string attribute = str.substr(0, index);
			std::string value = str.substr(index + 1);
			attributes[attribute] = value;
		}
		else
			break;
	}
	displayString("Peer(Client):" + attributes["destPort"] + " Receiving " + Path::getName(attributes["fileName"], true) + " received acknowledgement from Peer(Server):" + attributes["sourcePort"]);
	return true;
}

//----< interpreting file handling request message >------------------------------
void Receiver::interpretingReceivingMessage1(vector<string> storage, string text)
{
	Message msg("FILE_HANDLING_REPLY");
	map<string, string>& attributes = msg.getAttributes();
	for (auto str : storage)
	{
		size_t index = str.find(":");
		std::string attribute = str.substr(0, index);
		std::string value = str.substr(index + 1);
		attributes[attribute] = value;
	}
	string fileName = Path::getName(attributes["fileName"], true);
	displayString("Server receiving one buffer of " + fileName + " from Client");

	int length=1024;
	contentSaving(text, attributes["contentLength"]);
	if (!(attributes["contentLength"] == "1024"))
		length = stoi(attributes["contentLength"]);
	FileSystem::File::byte buffer[1024];
	for (int i = 0; i < 1024; i++)
	{
		if (i < length)
			buffer[i] = text[i];
		else
			buffer[i] = ' ';
	}
	string x(buffer, length);
	std::ofstream outfile;
	outfile.open(path + fileName, std::ios_base::app);
	outfile << x;
	if (!(attributes["contentLength"] == "1024"))
	{
		string temp = attributes["sourceIP"];
		attributes["sourceIP"] = attributes["destIP"];
		attributes["destIP"] = temp;
		string temp1 = attributes["sourcePort"];
		attributes["sourcePort"] = attributes["destPort"];
		attributes["destPort"] = temp1;
		attributes["body"] = "file Received Successfully";
		string fs = Path::fileSpec(path, fileName);
		string filePath = Path::getFullFileSpec(fs);
		displayString("\nServer has Received file Path:" + filePath);
		rQueue->enQ(msg);//enQ message after receiving whole file
	}
}

//----< test file handling Request >--------------------------------------
bool Receiver::fileHandlingRequest(Socket& socket_)
{
	std::vector<std::string> receiverStorage;
	for (int i = 0; i < 7; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "\n")
				break;
			else
				receiverStorage.push_back(str);
		}
		else
			break;
	}
	string receivedContent;
	if (!receivingBuffer(socket_, receivedContent))
		return false;
	interpretingReceivingMessage1(receiverStorage, receivedContent);
	return true;
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_RECEIVER
int main()
{
	title("Testing Receiver", '=');
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(9080, Socket::IP6);
		BlockingQueue<Message> bq;
		Receiver cp(&bq,"../TESTFOLDER2");
		sl.start(cp);
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
#endif

