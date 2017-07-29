#pragma once
/////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple one-way HTTP messaging          //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*
* Public Interface:
* This method awaits a response from server. Behaves like http get verb.
* std::string getMessage(std::string message, HttpMessageType httpMessageType);
*
* This method justs posts data to server like http verb post from server.
* void postMessage(std::string message, string requestData, HttpMessageType httpMessageType);

* Required Files:
*   MsgClient.h, MsgClient.cpp
*   HttpMessage.h, HttpMessage.cpp	//	For
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*
*/

#include <iostream>
#include "ClientWrapper.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "Configuration.h"
#include "../HttpMessage/HttpMessageTypes.h"

using namespace std;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//

class ClientCounter
{
public:
	ClientCounter() { ++clientCount; }
	size_t count() { return clientCount; }
private:
	static size_t clientCount;
};

/////////////////////////////////////////////////////////////////////
// Connection class
// - This class is used for communication with the server to send and
//	receive msgs.
//   
//

class Connection
{
public:
	using EndPoint = std::string;
	Connection(string serverURL, int port);
	//	Request response.
	std::string getMessage(std::string message, HttpMessageType httpMessageType);
	//	just request the server. (upload files thorugh this)
	void postMessage(std::string message, string requestData, HttpMessageType httpMessageType);
private:
	void DownloadFiles(string fileName, Socket & socket, string fqn = "");
	bool uploadFile(string filename, Socket & socket, string fqn = "");
	HttpMessage makeMessage(HttpMessageType, const EndPoint& ep, const std::string& resource, const std::string& msgBody = "");
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& filename, Socket& socket, string path = "", bool isDir = false);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	Configuration serverConfiguration;
};