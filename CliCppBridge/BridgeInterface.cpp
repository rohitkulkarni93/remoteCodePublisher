#include <iostream>
#include <string>
#include "BridgeInterface.h"
#include "../MockChannel/MockChannel.h"
#include <fstream>

using namespace System;

String ^ Bridge::GetResponse()
{
	return convertToSystemString(receiver->getMessage());
}

void Bridge::PostRequest(String^ resource, String^ data, String^ command, String ^ method)
{
	Request request;
	request.resource = convertToNativeString(resource);
	request.command = convertToNativeString(command);
	request.method = convertToNativeString(method);
	request.data = convertToNativeString(data);
	sender->postMessage(request);
}

void Bridge::Configure(String^ url, Int32 port)
{
	this->url = url;
	this->port = port;
	startChannel();
}

void Bridge::startChannel()
{
	ObjectFactory factory;
	sender = factory.createSendr(convertToNativeString(url), port);
	receiver = factory.createRecvr();
	channel = factory.createMockChannel(sender, receiver);
	channel->start();
}

std::string Bridge::convertToNativeString(String ^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}

String ^ Bridge::convertToSystemString(std::string str)
{
	std::cout << str;
	std::fstream fileWriter; fileWriter.open("logg.txt"); fileWriter << str; fileWriter.close();
	String ^ strconv = gcnew String(str.c_str());
	Console::WriteLine(strconv);
	return strconv;
}

