#pragma once
//////////////////////////////////////////////////////////////////////
// RecordStructure.h - Contains the DB record framework.			//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains the structure of the BaseRecord which is the fundamental
* component of the application. The record framework consists mainly of an abstract
* BaseRecord<T> class which contains all the metadata information related to the 
* record. Then a level below sits the Record<T> which derived from the base class
* and which is also abstract. The purpose of making them abstract is compile time type
* safety for the template parameter "T". Unless the specialized record structure 
* doesn't provide implementations for the DBEngine to successfully query it, the
* application will not compile.
*
* Required Files:
* ---------------
*   - RecordStructure.h
*
* Package Dependency:
* -------------------
*	- This package is one of the core components should compile before any other.
*
* Interfaces and Types:
* ---------------------
* ver 1.0 (Jan 2017)
*		- BaseRecord<T> : This type is the base of the record class. It contains
*						  the metadata information for a record and offers getters and 
*						  setters for accessing the information. There is no business
*						  logic which goes into the this type unless it is very useful.
*		- Record<T>		: This type derives from the BaseRecord type. This class cant be
*						  instanciated. It is just a wrapper around the BaseRecord class.
*		- Record<string>: This type is a template specialization which also derives from
*						  the base class. This class implements the virtual methods for
*						  converting the "T" data to and from XML.
*
* Maintenance History:
* --------------------
* ver 1.0 - Jan 2017
* Created the package.
*
* TODO:
* -----
* Use CppProperty package by professor instead of the getter and setter methods used for 
* the data members.
*/

#include <iostream>
#include <vector>
#include "../XmlDocument/XmlDocument.h"
#include "GlobalConstants.h"
#include "CoreInterfaces.h"
#include <algorithm>

using namespace std;

//	Base class for constructing a record object.
template <class T>
class BaseRecord {

public:
	string getItem() const {
		return item;
	}

	void setItem(const char * itemValue) {
		item = itemValue;
	}

	string getCategory() const {
		return category;
	}

	void setCategory(const char * categoryValue) {
		category = categoryValue;
	}

	string getDescription() const {
		return description;
	}

	void setDescription(const char * descriptionValue) {
		description = descriptionValue;
	}

	T getData() const {
		return data;
	}

	void setData(T dataObj) {
		data = dataObj;
	}

	void addRelationship(string key) {
		children.push_back(key);
	}

	void deleteRelationship(string key) {
		std::remove(children.begin(), children.end(), key);
	}

	const vector<string> & getChildKeys() {
		return children;
	}

	bool isKeyPresent(string key) {
		auto iter = find(children.begin(), children.end(), key);
		return iter != children.end();
	}

	string getTime() const {
		return time;
	}

	void setTime(const char * timeValue) {
		time = timeValue;
	}

private:

	string item;				//	Metadata
	string category;			//	Metadata
	string description;			//	Metadata
	string time;				//	Metadata
	vector<string> children;	//	Metadata
	T data;

	//	Construct object
	virtual void convertDataToXML(SPtr & xmlDocument) = 0;
	virtual T convertXMLToData(SPtr xmlDocument) = 0;
};

//	This is a record class which is a wrapper on top of the BaseRecord.
//	This class cannot be instanciated since it doesn't provide implementation 
//	for the pure virtual functions. Due to this, if a random structure is 
//	passed in the template which doesnt have an implementation, the application
//  will not compile and force the user to provide the implementation by 
//	creating a specialized template implementation for the methods.
template <class T>
class Record : public BaseRecord<T> {

};

//	Specialized recorsd class for string datatype.
//	This class provides an implementation for the string class.
template<>
class Record<std::string> : public BaseRecord<string>, public IRecordFormatter {
public:
	Record() { }
	/*Record(Record<string> &&) { }
	Record(const Record<string>&) = default;
	Record<string>& operator=(const Record<string>&) = default;
*/
	void convertDataToXML(SPtr & xmlDocument);
	string convertXMLToData(SPtr xmlDocument);
	string formatRecord(SelectObjectType objectType, string keyValue);

private:

};