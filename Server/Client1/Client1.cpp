#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include "Client1.h"
#include <iostream>

using namespace ApplicationHelpers;
using namespace Display;
using namespace std;

//----< Dequeue message from the receiving queue >-----------------
void getQueueMessage(BlockingQueue<Message>* rQueue, Sender* sd)
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

//----< build test string >-----------------
string buildTestString()
{
	string str;
	int count = 97;
	for (int i = 0; i < 1000; i++)
	{
		if (count > 122)
			count = 97;
		char asciiChar = count;
		str = str + asciiChar;
		count++;
	}
	return str;
}

//----< initialize client instance >-----------------
Client1::Client1(string sourceIP, int sourcePort, string destIP, int destPort,string filePath)
{
	title("Testing Socket Client", '=');
	displayPeerInformation("Client", sourceIP, sourcePort);
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(sourcePort, Socket::IP6);
		BlockingQueue<Message> rQueue; //receiving queue
		BlockingQueue<Message> sQueue; //sending queue
		Receiver rc(&rQueue,"");
		Sender sd(&sQueue);
		sl.start(rc);

		string test = buildTestString();
		Message msg("STRING_HANDLING_REQUEST");
		msg.buildMessage(msg, sourcePort, destPort, test, test.size());

		displayMessage(msg,"Constructing");
		Message msg2("FILE_HANDLING_REQUEST");
		msg.buildMessage(msg2, sourcePort, destPort, filePath);
		sQueue.enQ(msg);
		sQueue.enQ(msg2);

		std::thread sThread(&Sender::getQueueMessage, Sender(&sQueue));
		std::thread rThread(getQueueMessage, &rQueue, &sd);
		rThread.join();
		sThread.join();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< demonstration >--------------------------------------------------

#ifdef TEST_CLIENT1
int main()
{
	string fs = FileSystem::Path::fileSpec("..\\TESTFOLDER1\\", "text.txt");
	string path = FileSystem::Path::getFullFileSpec(fs);
	Client1 cl("localhost", 1000,"localhost", 2000,path);
}
#endif