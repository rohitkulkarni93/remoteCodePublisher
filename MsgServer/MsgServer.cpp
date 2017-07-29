/////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates simple one-way HTTP style messaging    //
//                 and file transfer                                   //
//                                                                     //
// Rohit Kulkarni, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
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
*/
/*
 *  
 *
 *
 */
#include "TestExecutiveProject4.h"
#include "../Sockets/Sockets.h"
#include "../HttpMessage/HttpMessage.h"
#include "../FileSystem/FileSystem.h"
#include "../VersionMismatch/Logger/Cpp11-BlockingQueue.h"
#include "../VersionMismatch/Logger/Logger.h"
#include "../VersionMismatch/Utilities/Utilities.h"
#include "../CodePublisher/CodePublisher.h"
#include "../CodeAnalyzer/Executive.h"
#include "../CodeAnalyzer/DepAnalysisEngine.h"
#include "../CodeAnalyzer/TypeTable.h"
#include "../CodeAnalyzer/TypeAnalysis.h"
#include "ServerConfiguration.h"
#include <thread>
#include <string>
#include <iostream>

using namespace VersionMismatch;
using Show = StaticLogger<1>;
using namespace std;

/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You no longer need to be careful using data members of this class
//   because each client handler thread gets its own copy of this 
//   instance so you won't get unwanted sharing.
// - I changed the SocketListener semantics to pass
//   instances of this class by value for version 5.2.
// - that means that all ClientHandlers need copy semantics.
//
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ, ServerConfiguration config) : msgQ_(msgQ) { init(config); }
	void operator()(Socket socket);
	void processMessage(Socket & socket, HttpMessage msg);
private:
	enum StreamMode {
		String,
		File
	};
	void StreamFiles(Socket & socket, HttpMessage msg);
	void init(ServerConfiguration);
	string checkAndCreateDirectory(string directory);
	string getCurrentDirectoryForBacktracking(string & response);
	string findServerMapping(string directory);
	void deleteFileFolder(HttpMessage msg, bool deleteDirectory);
	void recursiveDirectoryDelete(string path);
	void publishSourceCode(Socket & socket, HttpMessage msg);
	void browseServerDirectory(Socket & socket, HttpMessage msg);
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	void sendData(Socket & socket, string data, StreamMode mode);
	BlockingQueue<HttpMessage>& msgQ_;
	const ServerConfiguration * configuration;
};
//----< this defines processing to frame messages >------------------

void ClientHandler::init(ServerConfiguration config)
{
	//	Read configuration
	ServerConfiguration * obj = new ServerConfiguration;
	obj->wwwroot = checkAndCreateDirectory(config.wwwroot);
	obj->uploadedFilesDir = checkAndCreateDirectory(config.wwwroot + config.uploadedFilesDir);
	obj->publishedFilesDir = checkAndCreateDirectory(config.wwwroot + config.publishedFilesDir);
	if (config.port > -1 && config.port < 65535) {
		obj->port = config.port;
	}
	if (config.url.length() > 0) {
		obj->url = config.url;
	}
	this->configuration = obj;
	//	Set current directory.
	FileSystem::Directory::setCurrentDirectory(configuration->wwwroot);
}

string ClientHandler::checkAndCreateDirectory(string directory)
{
	if (directory.length() > 0)
	{
		bool exists = FileSystem::Directory::exists(directory);
		if (!exists)
			FileSystem::Directory::create(directory);
		return FileSystem::Path::getFullFileSpec(directory);
	}
	return "";
}


HttpMessage ClientHandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;
	HttpMessage msg;
	// read message attributes
	while (true)
	{
		string attributes = socket.recvString('\n');
		if (attributes.size() > 1)
		{
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attributes);
			msg.addAttribute(attrib);
		}
		else { break; }
	}
	// If client is done, connection breaks and recvString returns empty string
	if (msg.attributes().size() == 0)
	{
		connectionClosed_ = true;
		return msg;
	}
	if (msg.findValue("HttpMessageType") == "HMT_UPLOADFILE")
	{
		//	Handle file saving directly to file because file 
		//	can be large in size, no need to load it in memory
		std::string filename = msg.findValue("resource");
		if (filename != "")
		{
			size_t contentSize;
			std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;

			string dir = configuration->uploadedFilesDir;
			if (msg.findValue("directory").length() > 0 && msg.findValue("directory") != "$") {
				dir += msg.findValue("directory");
			}
			if (!FileSystem::Directory::exists(dir)) {
				FileSystem::Directory::create(dir);
			}
			FileSystem::Directory::setCurrentDirectory(dir);
			readFile(filename, contentSize, socket);
			FileSystem::Directory::setCurrentDirectory(configuration->wwwroot);

		}
		if (filename != "")
		{
			// construct message body
			msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			msg.addBody(bodyString);
		}
	}
	else
	{
		// read message body
		size_t numBytes = 0;
		size_t pos = msg.findAttribute("content-length");
		if (pos < msg.attributes().size())
		{
			numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}
	return msg;
}
//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message,
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
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
		Show::write("\n\n  can't open file " + fileName);
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
//----< receiver functionality is defined by this function >---------

void ClientHandler::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		processMessage(socket, msg);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

//	Publish Source code files.
void ClientHandler::publishSourceCode(Socket & socket, HttpMessage msg)
{
	using namespace CodeAnalysis;
	if (FileSystem::Directory::exists(findServerMapping(msg.findValue("resource"))))
	{
		string dirToPublish = findServerMapping(msg.findValue("resource"));
		FileSystem::Directory::setCurrentDirectory(dirToPublish);
		CodeAnalysisExecutive exec;
		try {
			exec.setParameters(dirToPublish, vector<string> { "*.h", "*.cpp" }, vector<char>{ });
			exec.getSourceFiles();
			exec.processSourceCode(true);
			exec.complexityAnalysis();
			exec.dispatchOptionalDisplays();
			exec.flushLogger();
			exec.displayMetricSummary(50, 10);

			exec.flushLogger();
			TypeAnal typeAnalyzer;
			typeAnalyzer.doTypeAnal(exec.getAnalysisPath());
			TypeTable typetable = typeAnalyzer.exportTypeTable();

			DependencyAnalysisEngine engine;
			engine.importTypeTable(typetable);
			engine.startDependencyAnalysis(exec.getAnalysisPath().c_str());
			engine.exportAnalysisResultToFile(exec.getXMLPath(), exec.getAnalysisPath());

			Config config;
			config.cssFileName = "CPStyleSheet.css";
			config.jsFileName = "CPScript.js";
			CodePublisher publisher;
			publisher.configure(config);
			publisher.importDependencies(engine.exportAnalysisResult());
			publisher.publishCode(exec.getAnalysisPath(), exec.getCommandLineArgs("outputpath"), exec.getPatterns());
		}
		catch (std::exception& except)
		{
			exec.flushLogger();
			std::cout << "\n\n  caught exception in Executive::main: " + std::string(except.what()) + "\n\n";
			exec.stopLogger();
		}
	}
}

//	Process the HTTP Message
void ClientHandler::processMessage(Socket & socket, HttpMessage msg)
{
	string command = msg.findValue("HttpMessageType");
	if (command == "HMT_PUBLISH") {
		publishSourceCode(socket, msg);
	}
	else if (command == "HMT_DELETEFILE" || command == "HMT_DELETEFOLDER") {
		deleteFileFolder(msg, command == "HMT_DELETEFOLDER");
	}
	else if (command == "HMT_BROWSEDIR") {
		browseServerDirectory(socket, msg);
	}
	else if (command == "HMT_DOWNLOADFILE") {
		StreamFiles(socket, msg);
	}
}

//	Stream files to client.
void ClientHandler::StreamFiles(Socket & socket, HttpMessage msg)
{
	string file = findServerMapping(msg.findValue("resource"));
	if (FileSystem::File::exists(file))
	{
		//	Yet to implement.
	}
}
//	Handle delete file and folder.
void ClientHandler::deleteFileFolder(HttpMessage msg, bool deleteDirectory)
{
	if (deleteDirectory)
	{
		string directory = findServerMapping(msg.findValue("resource"));
		directory += "\\";
		if (FileSystem::Directory::exists(directory)
			&& directory != this->configuration->uploadedFilesDir
			&& directory != this->configuration->publishedFilesDir)
		{
			recursiveDirectoryDelete(directory);
		}
	}
	else
	{
		string file = findServerMapping(msg.findValue("resource"));
		FileSystem::File::remove(file);
	}
}

//	delete directory recursively.
void ClientHandler::recursiveDirectoryDelete(string path)
{
	vector<string> dirs = FileSystem::Directory::getDirectories(path);
	dirs.erase(dirs.begin());	//	Delete . and .. which are at index 0 and 1.
	dirs.erase(dirs.begin());
	vector<string> files = FileSystem::Directory::getFiles(path);
	for (unsigned int i = 0; i < dirs.size(); i++)
		recursiveDirectoryDelete(path + "\\" + dirs[i]);
	for (unsigned int i = 0; i < files.size(); i++)
		FileSystem::File::remove(path + "\\" + files[i]);
	FileSystem::Directory::remove(path);
}

//	Browse server directory back and forth till mapped location.
void ClientHandler::browseServerDirectory(Socket & socket, HttpMessage msg)
{
	string response = "";
	if (msg.findValue("Method") == "GET") {
		//cout << "Got request for server directory." << endl;
		string serverMapping = findServerMapping(msg.findValue("resource"));
		if (FileSystem::Directory::exists(serverMapping)) {
			string backDirectory = getCurrentDirectoryForBacktracking(serverMapping);
			vector<string> dirs = FileSystem::Directory::getDirectories(serverMapping);
			vector<string> files = FileSystem::Directory::getFiles(serverMapping);
			if (serverMapping == configuration->wwwroot || backDirectory == configuration->wwwroot)
				response += "<$>";
			else {
				response += "<$\\" + backDirectory + ">";
			}
			for (unsigned int i = 0; i < dirs.size(); i++) {
				response += "<" + dirs[i] + ">";
			}
			response += "|";
			for (unsigned int i = 0; i < files.size(); i++) {
				response += "<" + files[i] + ">";
			}
			response += "?";
			sendData(socket, response, StreamMode::String);
		}
	}
}

//	Find the server mapping.
string ClientHandler::findServerMapping(string directory)
{
	string dirBase = "$\\";
	int index = directory.find(dirBase);
	if (index > -1 && index + dirBase.length() < directory.length()) {
		return configuration->wwwroot + directory.substr(index + dirBase.length());
	}
	return configuration->wwwroot;
}

string ClientHandler::getCurrentDirectoryForBacktracking(string & response)
{
	int index = response.find(configuration->wwwroot);
	int count = -1, ind = 0;
	while (ind != -1)
	{
		count++;
		ind = response.find("../", ind + 1);
	}
	if (count > 0)
	{
		count++;
		while (count > 0)
		{
			response = response.substr(0, response.find_last_of('\\'));
			count--;
		}
		if (response.find(configuration->wwwroot) == -1)
			response = configuration->wwwroot;
	}
	if (index > -1 && index + configuration->wwwroot.length() < response.length())
	{
		return response.substr(index + configuration->wwwroot.length());
	}
	return configuration->wwwroot;
}

void ClientHandler::sendData(Socket & socket, string data, StreamMode mode)
{
	if (mode == StreamMode::File) {

	}
	else {
		cout << "sending data to client";
		size_t length = data.length();
		string dataLength = Converter<int>::toString(length);
		socket.sendString(dataLength, '\n');
		socket.sendString(data, '\n');
		cout << "sent successfully.";
	}
}

//----< test stub >--------------------------------------------------

int main()
{
	::SetConsoleTitle(L"HttpMessage Server - Runs Forever");

	BlockingQueue<HttpMessage> msgQ;

	try
	{
		TestExecutiveProject4 exec;
		TestExecutiveProject4::Print("Demonstrating Req 1");
		TestExecutiveProject4::Req1();
		TestExecutiveProject4::Print("Demonstrating Req 2");
		TestExecutiveProject4::Req2();
		TestExecutiveProject4::Print("Demonstrating Req 3");
		TestExecutiveProject4::Req3();
		TestExecutiveProject4::Print("Demonstrating Req 4");
		TestExecutiveProject4::Req4();
		TestExecutiveProject4::Print("Demonstrating Req 5");
		TestExecutiveProject4::Req5();
		TestExecutiveProject4::Print("Demonstrating Req 6");
		TestExecutiveProject4::Req6();
		TestExecutiveProject4::Print("Demonstrating Req 7");
		TestExecutiveProject4::Req7();
		TestExecutiveProject4::Print("Demonstrating Req 8");
		TestExecutiveProject4::Req8();
		TestExecutiveProject4::Print("Demonstrating Req 9");
		TestExecutiveProject4::Req9();
		Show::attach(&std::cout);
		Show::start();
		Show::title("\n  HttpMessage Server started");
		ServerConfiguration config;
		config.port = 8060;
		config.url = "localhost";
		config.wwwroot = "./wwwroot\\";
		config.publishedFilesDir = "publish\\";
		config.uploadedFilesDir = "upload\\";
		SocketSystem ss;	//	Initialize socket system.
		SocketListener sl(config.port, Socket::IP6);	//	Start listening on this port.
		ClientHandler cp(msgQ, config);	//	create a handler.
		sl.start(cp);	//	start server
		/*
		 * Since this is a server the loop below never terminates.
		 * We could easily change that by sending a distinguished
		 * message for shutdown.
		 */
		while (true)
		{

		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}