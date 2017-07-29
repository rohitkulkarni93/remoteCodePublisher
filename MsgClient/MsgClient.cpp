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
 */
 /*
 * Required Files:
 *   MsgClient.cpp, MsgServer.cpp
 *   HttpMessage.h, HttpMessage.cpp
 *   Cpp11-BlockingQueue.h
 *   Sockets.h, Sockets.cpp
 *   FileSystem.h, FileSystem.cpp
 *   Logger.h, Logger.cpp
 *   Utilities.h, Utilities.cpp
 *
 *
 *
 */
#include "CommandTranslator.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../VersionMismatch/Utilities/Utilities.h"
#include <string>
#include <iostream>
#include "MsgClient.h"
#include "../HttpMessage/HttpMessageTypes.h"

using namespace VersionMismatch;
using Utils = StringHelper;
size_t ClientCounter::clientCount = 0;

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */

 //	Protocol:
 //	attributeAt 0 = toAddr
 //	attributeAt 1 = fromAddr
 //	attributeAt 2 = method/verb
 //	attributeAt 3 = command
HttpMessage Connection::makeMessage(HttpMessageType messageType, const EndPoint& ep, const std::string& resource, const std::string& body)
{
	cout << "Make message" << resource << " : Body " << body;
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute(string("fromAddr:") + "getMefromClient"));
	msg.addAttribute(HttpMessage::parseAttribute("resource:" + resource));
	msg.addAttribute(HttpMessage::attribute("Method", "POST"));
	switch (messageType)
	{
	case HttpMessageType::HMT_UPLOADFILE:
		cout << "Browser dir added.";
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_UPLOADFILE"));
		break;
	case HttpMessageType::HMT_UPLOADDIR:
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_UPLOADDIR"));
		break;
	case HttpMessageType::HMT_PUBLISH:
		cout << "Browser dir added.";
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_PUBLISH"));
		msg.addBody(body);
		break;
	case HttpMessageType::HMT_DELETEFILE:
		cout << "Browser dir added.";
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_DELETEFILE"));
		break;
	case HttpMessageType::HMT_DELETEFOLDER:
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_DELETEFOLDER"));
		break;
	case HttpMessageType::HMT_BROWSEDIR:
		cout << "sdfsdfsdfsdfsdf";
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_BROWSEDIR"));
		msg.removeAttribute("Method");
		msg.addAttribute(HttpMessage::attribute("Method", "GET"));
		break;
	case HttpMessageType::HMT_DOWNLOADFILE:
		msg.removeAttribute("Method");
		msg.addAttribute(HttpMessage::attribute("Method", "GET"));
		msg.addAttribute(HttpMessage::attribute("HttpMessageType", "HMT_DOWNLOADFILE"));
		break;
	default:
		cout << "Error unknown";
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
		break;
	}
	msg.addBody(body);
	if (body.size() > 0)
	{
		attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
		msg.addAttribute(attrib);
	}
	return msg;
}
//----< send message using socket >----------------------------------
Connection::Connection(string serverURL, int port)
{
	this->serverConfiguration.serverAddr = serverURL;
	this->serverConfiguration.port = port;
	cout << "Config : " << serverConfiguration.serverAddr << serverConfiguration.port;
}

void Connection::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	cout << endl << endl << "Sending final string : " << msgString;
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */

bool Connection::sendFile(const std::string& filename, Socket& socket, string category, bool isDir)
{
	// assumes that socket is connected
	if (isDir)
	{
		cout << "Request to upload directory:" << filename;
		if (!FileSystem::Directory::exists(filename))
			return false;
		string path = FileSystem::Path::getPath(filename);
		path = path.substr(0, path.length() - 1);
		size_t lastIndex = path.find_last_of('\\') + 1;
		if (lastIndex < path.length())
			path = path.substr(lastIndex, path.length() - lastIndex);
		if (category.length() > 0)
		{
			path = category;
		}
		vector<string> files = FileSystem::Directory::getFiles(filename, "*");
		cout << endl << "#files:" << files.size();
		for (int i = 0; i < files.size(); i++) {
			string fullyQualifiedName = FileSystem::Path::getPath(filename) + files[i];
			cout << "File:" << fullyQualifiedName;
			uploadFile(fullyQualifiedName, socket, path);
		}
		return true;
	}
	else {
		if (FileSystem::File::exists(filename)) {
			if (category.length() > 0)
				uploadFile(filename, socket, category);
			else
				uploadFile(filename, socket, "$");
			return true;
		}
	}
	return false;
}

bool Connection::uploadFile(string filename, Socket & socket, string dir)
{
	FileSystem::FileInfo fi(filename);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(filename);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	cout << "Trying to upload file passed all checks till here";
	filename = filename.substr(filename.find_last_of("\\") + 1);
	HttpMessage msg = makeMessage(HttpMessageType::HMT_UPLOADFILE, "localhost:8081", filename);
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	msg.addAttribute(HttpMessage::Attribute("directory", dir));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}

//----< this defines the behavior of the client >--------------------

void Connection::postMessage(std::string message, string requestData, HttpMessageType httpMessageType)
{
	SocketSystem ss;
	SocketConnecter si;
	cout << "Trying to connect to server : " << serverConfiguration.serverAddr << serverConfiguration.port;
	while (!si.connect(serverConfiguration.serverAddr, serverConfiguration.port))
	{
		::Sleep(100);
	}
	if (httpMessageType == HttpMessageType::HMT_UPLOADFILE || httpMessageType == HttpMessageType::HMT_UPLOADDIR)
		sendFile(message, si, requestData, httpMessageType == HttpMessageType::HMT_UPLOADDIR);
	else
	{
		HttpMessage msg;
		msg = makeMessage(httpMessageType, "localhost:8080", message);
		sendMessage(msg, si);
	}
}

std::string Connection::getMessage(std::string message, HttpMessageType httpMessageType)
{
	SocketSystem ss;
	SocketConnecter si;
	cout << "Trying to connect to server : " << serverConfiguration.serverAddr << " " << serverConfiguration.port;
	while (!si.connect(serverConfiguration.serverAddr, serverConfiguration.port))
	{
		::Sleep(100);
	}
	HttpMessage msg;
	msg = makeMessage(httpMessageType, "localhost:8080", message, "");
	cout << "Header length: " << msg.headerString().size();
	sendMessage(msg, si);
	string response = CommandTranslator::Translate(httpMessageType) + ":";
	::Sleep(100);
	if (httpMessageType == HttpMessageType::HMT_DOWNLOADFILE)
		DownloadFiles("", si, "");
	else {
		bool ready = si.waitForData(5000, 100);
		if (ready)
		{
			size_t responseLength = Converter<int>::toValue(si.recvString('\n'));
			if (responseLength > 0) {
				bool ready = si.waitForData(5000, 100);
				if (ready) {
					char * buffer = new char[responseLength + 1];
					si.recvStream(responseLength, buffer);
					buffer[responseLength] = '\0';
					response += buffer;
				}
			}
		}
	}
	return response;
}

//	Download files
void Connection::DownloadFiles(string fileName, Socket & socket, string fqn)
{
	int totalFiles = Converter<int>::toValue(socket.recvString('\n'));
	string fName = socket.recvString('\n');
	if (fName.length() > 0)
	{
		//	No time to implement
		//cout << fName << " -filename";
		//size_t contentSize;
		//string sizeString = socket.recvString('\n');
		//if (sizeString != "")
		//	contentSize = Converter<size_t>::toValue(sizeString);
		//else
		//	return;

		//readFile(fName, contentSize, socket);
	}
}

bool Connection::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	string fileName = filename;
	FileSystem::File file(fileName);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		/*
		* This error handling is incomplete.  The client will continue
		* to send bytes, but if the file can't be opened, then the server
		* doesn't gracefully collect and dump them as it should.  That's
		* an exercise left for students.
		*/
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}
