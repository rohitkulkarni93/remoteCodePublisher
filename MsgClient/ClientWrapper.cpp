//#include "ClientWrapper.h"
//#include "MsgClient.h"
//#include "../Sockets/Sockets.h"
//#include "CommandTranslator.h"
//#include <fstream>
//
////	Forward declared in ClientWrapper.h
////	This is done to avoid refering the SocketConnector type
////	in the ClientWrapper.h file.
//struct HttpOverSocketChannel::SocketWrapper {
//	//	Replace this with a smart pointer.
//	Connection * connectionObj;
//};
//
////	Issue a get i.e. receive a response from the server
////	Could be a string or an entire file.
//std::string HttpOverSocketChannel::get(Resource res, Command command)
//{
//	return wrapper->connectionObj->getMessage(res, CommandTranslator::Translate(command));
//}
//
////	Issue a post. i.e. No waiting for client to receive the call
//void HttpOverSocketChannel::post(Resource res, RequestData data, Command command)
//{
//	this->wrapper->connectionObj->postMessage(res, data, CommandTranslator::Translate(command));
//
//}
//
////	Set server address configuration.
//void HttpOverSocketChannel::configure(string serverURL, int port)
//{
//	this->serverConfig.serverURL = serverURL;
//	this->serverConfig.port = port;
//	if (this->wrapper && this->wrapper->connectionObj)
//	{
//		delete this->wrapper->connectionObj;
//		this->wrapper->connectionObj =
//			new Connection(this->serverConfig.serverURL, this->serverConfig.port);
//	}
//
//}
//
////	Ctor Creating new object of SocketConnector on the heap.
//HttpOverSocketChannel::HttpOverSocketChannel()
//{
//	this->configure("localhost", 8080);
//	this->wrapper = new SocketWrapper();
//	this->wrapper->connectionObj = 
//		new Connection(this->serverConfig.serverURL, this->serverConfig.port);
//}
//
////	Destructor
//HttpOverSocketChannel::~HttpOverSocketChannel()
//{
//	delete this->wrapper->connectionObj;
//	this->wrapper->connectionObj = nullptr;
//	this->wrapper = nullptr;
//}
//
////	Method used for intercepting the requests and logging them.
//void HttpOverSocketChannel::logRequest(std::string msg)
//{
//	fstream fileWriter;
//	fileWriter.open("log.txt", ios::out);
//	fileWriter << msg << endl;
//	fileWriter.close();
//}
//
////	Simple factory to instanciate channel and configure it the way we need.
//IChannel * ChannelFactory::getChannel(std::string channel)
//{
//	return new HttpOverSocketChannel;
//}