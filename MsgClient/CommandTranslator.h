#pragma once
//////////////////////////////////////////////////////////////////////
// CommandTranslator.h - Translates httpStyle commands				//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package translates commands from string to enum and back.
*
*
* Required Files:
* ---------------
*   - CommandTranslator.h
*
* Maintenance History:
* --------------------
* ver 1.0 - Jan 2017
* Created the package and added the basic interfaces and classes.
*
* TODO:
* -----
* Plan to add logging interface for the database engine.
*/

#include "../HttpMessage/HttpMessageTypes.h"
#include <iostream>

using namespace std;

class CommandTranslator
{
public:
	static HttpMessageType Translate(string command)
	{
		HttpMessageType messageType = HttpMessageType::HMT_UNDEFINED;
		if (command == "HMT_UPLOADFILE") {
			messageType = HttpMessageType::HMT_UPLOADFILE;
		}
		if (command == "HMT_UPLOADDIR") {
			messageType = HttpMessageType::HMT_UPLOADDIR;
		}
		else if (command == "HMT_DELETEFOLDER") {
			messageType = HttpMessageType::HMT_DELETEFOLDER;
		}
		else if (command == "HMT_BROWSEDIR") {
			messageType = HttpMessageType::HMT_BROWSEDIR;
		}
		else if (command == "HMT_DEP") {
			messageType = HttpMessageType::HMT_DEP;
		}
		else if (command == "HMT_DELETEFILE") {
			messageType = HttpMessageType::HMT_DELETEFILE;
		}
		else if (command == "HMT_PUBLISH") {
			messageType = HttpMessageType::HMT_PUBLISH;
		}
		else if (command == "HMT_DOWNLOADFILE")
			messageType = HttpMessageType::HMT_DOWNLOADFILE;
		return messageType;
	}

	static string Translate(HttpMessageType message) {
		string messageType = "HMT_UNDEFINED";
		switch (message)
		{
		case HttpMessageType::HMT_BROWSEDIR:
			messageType = "HMT_BROWSEDIR";
			break;
		case HttpMessageType::HMT_DELETEFILE:
			messageType = "HMT_DELETEFILE";
			break;
		case HttpMessageType::HMT_DELETEFOLDER:
			messageType = "HMT_DELETEFOLDER";
			break;
		case HttpMessageType::HMT_DEP:
			messageType = "HMT_DEP";
			break;
		case HttpMessageType::HMT_PUBLISH:
			messageType = "HMT_PUBLISH";
			break;
		case HttpMessageType::HMT_UPLOADDIR:
			messageType = "HMT_UPLOADDIR";
			break;
		case HttpMessageType::HMT_UPLOADFILE:
			messageType = "HMT_UPLOADFILE";
			break;
		case HttpMessageType::HMT_DOWNLOADFILE:
			messageType = "HMT_DOWNLOADFILE";
			break;
		}
		return messageType;
	}
};

#ifdef COMANDTANNSLATOR_TEST
int main()
{
	//	Convert to enum.
	HttpMessageType msg = CommandTranslator::Translate("HMT_BROWSEDIR");
	//	Convert back to string.
	string str = CommandTranslator::Translate(msg);
}

#endif // COMANDTANNSLATOR_TEST
