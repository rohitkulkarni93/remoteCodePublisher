///////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015                         //
// - build as static library showing how C++\CLI client can use native code channel  //
// - MockChannel reads from sendQ and writes to recvQ                                //
//                                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015                         //
///////////////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "../VersionMismatch/Logger/Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include "../MsgClient/MsgClient.h"
#include "../MsgClient/CommandTranslator.h"

using namespace VersionMismatch;

using BQueue = BlockingQueue <Message>;
using ReqQueue = BlockingQueue <Request>;
/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//

class Sendr : public ISendr
{
public:
	void postMessage(const Request& msg);
	ReqQueue& queue();
	Message processMessage();
	void configure(string url, int port);
private:
	Connection * conn;
	ReqQueue sendQ_;
};

void Sendr::configure(string url, int port)
{
	cout << "configuration: " << url << port;
	this->conn = new Connection(url, port);
}

Message Sendr::processMessage()
{
	Request msg = sendQ_.deQ();
	HttpMessageType msgType = CommandTranslator::Translate(msg.command);
	if (msg.method == "GET")
	{
		cout << "executing get on the msg" << msg.resource;
		return conn->getMessage(msg.resource, msgType);
	}
	else 
	{
		cout << endl << endl << "executing post on the msg: " << msg.data;
		conn->postMessage(msg.resource, msg.data, msgType);
	}
	return "";
}

void Sendr::postMessage(const Request& msg)
{
	cout << "Enquing message + " << msg.command << msg.data << msg.method << msg.resource << endl;
	sendQ_.enQ(msg);
}

ReqQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	Message getMessage();
	BQueue& queue();
private:
	BQueue recvQ_;
};

Message Recvr::getMessage()
{
	return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
	return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout << "\n Channel starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		ReqQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_)
		{
			std::cout << "\n  channel deQing message";
			// will block here so send quit message when stopping
			Message response = pSendr->processMessage();
			if(response.length() > 0)
				recvQ.enQ(response);
		}
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr(string url, int port) {

	Sendr * sender = new Sendr();
	sender->configure(url, port);
	return (ISendr *)sender;
}

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
	pMockChannel->start();
	pSendr->postMessage("Hello World");
	pSendr->postMessage("CSE687 - Object Oriented Design");
	Message msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pSendr->postMessage("stopping");
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pMockChannel->stop();
	pSendr->postMessage("quit");
	std::cin.get();
}
#endif
