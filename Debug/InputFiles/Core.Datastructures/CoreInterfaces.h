#pragma once
//////////////////////////////////////////////////////////////////////
// CoreInterfaces.h - Contains all the shared core interfaces.		//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains all the Core Interfaces required across the application.
* Any new common interfaces which will be used should be added in this package.
* 
* CoreInterfaces.h		- File contains all the public interfaces.
*
* Required Files:
* ---------------
*   - CoreInterfaces.h
*
* Package Dependency:
* -------------------
*	- This package is one of the core components should compile before any other.
*	  Most of the packages in the application will use the interfaces and classes 
*	  declared in this package.
*
* Interfaces and Types:
* ---------------------
* ver 1.0 (Jan 2017)
*	- IOutputFormatter - This interface is used by the database engine to format
*						 all the output messages.
*	- ConsoleFormatter - This class implements the IOutputFormatter interface. It provides
*						 definitions for outputting to the system.console.
*	- IRecordFormatter - This interface provides a way for record to format its output.
*						 The specialized record class needs to implement it.
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
#include <iostream>
#include "../XmlDocument/XmlDocument.h"
#include "GlobalConstants.h"
#include <unordered_map>

using namespace std;

//	This interface will be implemented by the record class.
class IRecordFormatter {
public:
	//	This method will be invoked on firing the select query.
	virtual string formatRecord(SelectObjectType obj, string keyValue) = 0;
};

class IOutputFormatter {
public:
	//	This method will be used for showing exceptions.
	virtual void showException(OperationOutcome exceptionCode, string identifier) = 0;
	//	This method will be used for showing regular messages.
	virtual void showMessage(string message) = 0;
	//	This method will be used for showing records.
	virtual void showRecord(IRecordFormatter & rec, SelectObjectType objectType, string key = "") = 0;
};

//	This class handles all the output messages to system.console.
class ConsoleFormatter : public IOutputFormatter {
public:
	void showException(OperationOutcome exceptionCode, string identifier);
	void showMessage(string message);
	void showRecord(IRecordFormatter & rec, SelectObjectType objectType, string key = "");
};

class IDataReader {
public:
	template <typename T>
	T readField(string fieldName) = 0;
};