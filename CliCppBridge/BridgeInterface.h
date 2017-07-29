#pragma once

#include <string>
#include "../MockChannel/MockChannel.h"

using namespace System;

public ref class Bridge
{
public:
	void PostRequest(String  ^ resource, String  ^ data, String ^ command, String ^ method);
	String ^ GetResponse();
	void Configure(String^ url, Int32 port);
private:
	void startChannel();
	std::string convertToNativeString(String ^ str);
	String ^ convertToSystemString(std::string str);
	String ^ url = "";
	int port;
	ISendr * sender;
	IRecvr * receiver;
	IMockChannel * channel;
};