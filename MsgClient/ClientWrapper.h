//#pragma once
//#include <string>
//using namespace std;
//
//using Resource = std::string;
//using Response = std::string;
//using RequestData = std::string;
//using Command = std::string;
//
//class IChannel
//{
//public:
//	virtual void configure(string serverUrl, int port) = 0;
//	virtual Response get(Resource resource, Command command) = 0;
//	virtual void post(Resource resource, RequestData data, Command command) = 0;
//};
//
//class HttpOverSocketChannel : public IChannel
//{
//	struct Config {
//		string serverURL;
//		int port;
//	};
//public:
//	struct SocketWrapper;
//	HttpOverSocketChannel();
//	~HttpOverSocketChannel();
//	void configure(std::string serverURL, int port);
//	std::string get(Resource resource, Command command);
//	void post(Resource resource, RequestData data, Command command);
//private:
//	void logRequest(std::string msg);
//	bool log = false;
//	Config serverConfig;
//	SocketWrapper * wrapper;
//};
//
//class ChannelFactory
//{
//public:
//	IChannel * getChannel(std::string channel);
//};