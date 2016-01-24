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
	string str = "Written in these walls are the stories that I can not explain I leave my heart open but it stays right here";
	string str1 = " empty for days She told me in the morning she don't feel the same about us in her bones";
	string str2 = " Seems to me that when I die these words will be written on my stone And I will be gone, gone tonight ";
	string str3="The ground beneath my feet is open wide The way that I been holding on too tight With nothing in between";
	
	return str+str1+str2+str3;
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

		Message msg1("FILE_HANDLING_REQUEST");
		msg.buildMessage(msg1, sourcePort, destPort, filePath);

		//string fs = FileSystem::Path::fileSpec(filePath, "");
		string filepath1 = FileSystem::Path::getPath(filePath) + "karan.jpg";
		Message msg2("FILE_HANDLING_REQUEST");
		msg.buildMessage(msg2, sourcePort, 2002, filepath1);

		sQueue.enQ(msg);
		sQueue.enQ(msg1);
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

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//----< demonstration >--------------------------------------------------

#ifdef TEST_CLIENT1
int main()
{
	string fs = FileSystem::Path::fileSpec("..\\TESTFOLDER1\\", "text.txt");
	string path = FileSystem::Path::getFullFileSpec(fs);
	Client1 cl("localhost", 1000,"localhost", 2000,path);
}
#endif