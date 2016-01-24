///////////////////////////////////////////////////////////////////////
// Display.cpp - display output on console							 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron 17, Windows 8                          //
// Application: Demonstration for CSE 687, Project #3, Spring 2015   //
// Author:      Karankumar Patel, Syracuse University                //
//              (315) 751-5637, khpatel@syr.edu                      //
///////////////////////////////////////////////////////////////////////

#include "../Message/Message.h"
#include "../Display/Display.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include <iostream>

using namespace ApplicationHelpers;
using namespace Display;

//----< dsiplay message on colsole >-----------------
void Display::displayMessage(Message& msg,string type)
{
	map<string, string> att = msg.getAttributes();
	displayLine('=');
	Verbose::show("Demonstrating: " + type+" Message", always);
	displayLine('=');
	Verbose::show(" Command: " + msg.getCommand(), always);
	Verbose::show(" SourceIP: " + att["sourceIP"], always);
	Verbose::show(" SourcePort: " + att["sourcePort"], always);
	Verbose::show(" DestIP: " + att["destIP"], always);
	Verbose::show(" DestPort: " + att["destPort"], always);
	Verbose::show(" ContentLength: " + att["contentLength"], always);
	displayLine('-');
}

//----< dsiplay string on colsole >-----------------
void Display::displayString(string str)
{
	Verbose::show(str, always);
}

//----< dsiplay line on colsole >-----------------
void Display::displayLine(char lineType)
{
	Verbose::show(string(79, lineType), always);
}

//----< dsiplay peer information colsole >-----------------
void Display::displayPeerInformation(string peerName, string ip, int port)
{
	string str = peerName + " will listen on IP: " + ip + " and Port: " + to_string(port);
	Verbose::show(str, always);
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_DISPLAY
int main()
{
	displayPeerInformation("Client", "localhost", 1000);
	string test = "this is test string";
	Message msg("STRING_HANDLING_REQUEST");
	msg.buildMessage(msg, 1000, 2000, test, test.size());
	displayMessage(msg, "Constructing");

	displayLine('=');
	displayString("Demonstrating: Sending string to one peer to other peer");
	displayLine('=');
}
#endif