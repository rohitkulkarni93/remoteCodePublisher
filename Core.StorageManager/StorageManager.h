#pragma once
//////////////////////////////////////////////////////////////////////
// StorageManager.h - Package used for file services.				//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is the Data Access Layer for the NoSQL database engine. The
* engine get services to persist data to XML files, and read and retrieve
* it back into memory.
* 
* IStorageManager is the interface which the DBEngine uses for communication
* with the storage manager.
*
* Note: Since the class is templated, all the functions in it are written in
* the StorageManager.h header file so as to avoid the LNK2019 error. Also a
* blank source file is included so the compiler compiles the project and outputs
* a static library.
* 
* Required Files:
* ---------------
*   - StorageManager.h
*	- Source.cpp (This file is blank and only included to generate the static library.
* 
* Types and interfaces:
* ---------------------
*	- IStorageManager : This interface is exposed out to the DBEngine to 
*						load and store data into the files. If a different storage 
*						mechanism is to be implemented (e.g. JSON) it can be done 
*						by implemening this interface.
* Dependency:
* -----------
*	- Convert.h : The Convert.h package (by Dr. James Fawcett) is required to 
*				  cast data from string to different data types.
*
* Maintenance History:
* --------------------
* ver 1.0 Jan 2017
* Created the package with basic functionality.
* 
* ToDo:
* -----
* Find a way to split the large class into two or more cpp files.
*/
#include <unordered_map>
#include <iostream>
#include <ctime>
#include "../Core.Datastructures/GlobalConstants.h"
#include "StorageManager.h"
#include "../Core.Datastructures/RecordStructure.h"
#include "../Core.Datastructures/DatabaseStructure.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../Convert/Convert.h"
#include "../Core.Datastructures/StrHelper.h"
#include <fstream>
#include "../Core.Datastructures/CoreInterfaces.h"
#include <vector>
using namespace XmlProcessing;
using namespace std;

//	Storage manager interface
template <typename T>
class IStorageManager {
public:
	//	Save the data base to file.
	virtual OperationOutcome saveToFile(unordered_map<string, Record<T>> data) = 0;
	//	Load the file contents to database.
	virtual unordered_map<string, Record<T>> loadDatabase() = 0;
	//	Initialize database.
	virtual bool initDatabase() = 0;
};

template <typename T>
class StorageManager : IStorageManager<T> {

public:
	StorageManager(DatabaseInfo & dbInfo, IOutputFormatter * outputFormatter);
	~StorageManager();
	OperationOutcome saveToFile(unordered_map<string, Record<T>> data);
	unordered_map<string, Record<T>> loadDatabase();
	bool initDatabase();
private:

	void createDBStructure(DatabaseInfo dbName, XmlProcessing::XmlDocument & xmlDocument);
	OperationOutcome saveDataToFile(unordered_map<string, Record<T>> data);
	string processDatabaseFile();
	unordered_map<string, Record<T>> processXMLToObjects(string xmlFile);
	void createRecordObjectKey(SPtr & dbRecord, string key);
	void createRecordValueMetadata(SPtr & dbRecord, Record<T> record);
	void createRecordValue(SPtr & dbRecord, Record<T> data);
	void createRecordChildKeyRelationships(SPtr & dbRecord, Record<T> data);
	void createRecordDataObject(SPtr & valuePointer, Record<T> & record);
	string processMetadataNode(MetadataType index, const SPtr & node);

#pragma region XML Constants
	const string FILEEXTENSION = ".db.xml";

	const string XMLDATABASE_TAG = "xmldatabase";
	const string METADATA_TAG = "metadata";
	const string AUTOCOMMIT_TAG = "autocommit";
	const string RECORDS_TAG = "records";
	const string RECORD_TAG = "record";
	const string OBJECTID_TAG = "objectId";
	const string VALUE_TAG = "value";
	const string DBNAME_TAG = "dbname";

	const string CATEGORY_TAG = "category";
	const string ITEM_TAG = "item";
	const string TIME_TAG = "time";
	const string DESCRIPTION_TAG = "description";
	const string CHILDREN_TAG = "children";

	const string KEYS_TAG = "keys";
	const string KEY_TAG = "key";
#pragma endregion
	DatabaseInfo * databaseInfo;
	IOutputFormatter * formatter;
};

template <typename T>
StorageManager<T>::StorageManager(DatabaseInfo & dbInfo, IOutputFormatter * outputFormatter) {
	this->databaseInfo = &dbInfo;
	this->formatter = outputFormatter;
};

template <typename T>
StorageManager<T>::~StorageManager() {
	//	Perform clean up
}

template <typename T>
OperationOutcome StorageManager<T>::saveToFile(unordered_map<string, Record<T>> data) {
	return saveDataToFile(data);
}

//	This method processes data from an .db.xml file and loads it in memory.
template <typename T>
unordered_map<string, Record<T>> StorageManager<T>::loadDatabase() {
	return processXMLToObjects(processDatabaseFile());
}

//	This method initializes the database
template <typename T>
bool StorageManager<T>::initDatabase() {
	try {
		XmlDocument document;
		createDBStructure(*(this->databaseInfo), document);
		string dbMetadata = document.toString();
		ofstream fileWriter;
		fileWriter.open(this->databaseInfo->DatabaseName + this->FILEEXTENSION, ios::out);
		fileWriter << dbMetadata;
		fileWriter.close();
		return true;
	}
	catch (...) {
		return false;
	}
};

//	This method saves the data from cursor into file.
template <typename T>
OperationOutcome StorageManager<T>::saveDataToFile(unordered_map<string, Record<T>> cursor) {
	try {

		XmlDocument xmlDocument;
		this->createDBStructure(*(this->databaseInfo), xmlDocument);
		SPtr ASTRoot = xmlDocument.docElement();
		SPtr pRoot = makeTaggedElement(this->RECORDS_TAG);
		for (auto it = cursor.begin(); it != cursor.end(); ++it) {

			SPtr dbRecord = makeTaggedElement(this->RECORD_TAG);
			//	Create record key.
			createRecordObjectKey(dbRecord, it->first);
			//	Create record value metadata.
			createRecordValueMetadata(dbRecord, it->second);
			//	Create record child relationships
			createRecordChildKeyRelationships(dbRecord, it->second);
			//	Create record value instance.
			createRecordValue(dbRecord, it->second);
			pRoot->addChild(dbRecord);

			//	Create record
			SPtr recordElement = makeTaggedElement(this->VALUE_TAG);
		}
		ASTRoot->addChild(pRoot);
		string xmlData = ASTRoot->toString();
		ofstream fileWriter(this->databaseInfo->DatabaseName + this->FILEEXTENSION);
		fileWriter << xmlData;
		fileWriter.close();
	}
	catch (...) {
		return OperationOutcome::Failure;
	}
	return OperationOutcome::CommitProcessed;
};

//	Set of methods below are used for processing record to XML
template <typename T>
void StorageManager<T>::createRecordObjectKey(SPtr & dbRecord, string objectid) {

	//	Create Key
	SPtr keyElement = makeTaggedElement(this->OBJECTID_TAG);
	SPtr keyValue = makeTextElement(objectid);
	keyElement->addChild(keyValue);
	dbRecord->addChild(keyElement);
}

template <typename T>
void StorageManager<T>::createRecordValueMetadata(SPtr & dbRecord, Record<T> record)
{
	//	Add Item property
	SPtr recordMetadata = makeTaggedElement(this->ITEM_TAG);
	recordMetadata->addChild(makeTextElement(record.getItem()));
	dbRecord->addChild(recordMetadata);

	//	Add category property
	recordMetadata = makeTaggedElement(this->CATEGORY_TAG);
	recordMetadata->addChild(makeTextElement(record.getCategory()));
	dbRecord->addChild(recordMetadata);

	//	Add time property
	recordMetadata = makeTaggedElement(this->TIME_TAG);
	recordMetadata->addChild(makeTextElement(record.getTime()));
	dbRecord->addChild(recordMetadata);

	//	Add description property
	recordMetadata = makeTaggedElement(this->DESCRIPTION_TAG);
	recordMetadata->addChild(makeTextElement(record.getDescription()));
	dbRecord->addChild(recordMetadata);
}

template <typename T>
void StorageManager<T>::createRecordChildKeyRelationships(SPtr & dbRecord, Record<T> record)
{
	SPtr recordKeys = makeTaggedElement(this->KEYS_TAG);
	vector<string> recordChildren = record.getChildKeys();
	for (auto it = recordChildren.begin(); it != recordChildren.end(); ++it) {
		SPtr key = makeTaggedElement(this->KEY_TAG);
		key->addChild(makeTextElement(*it));
		recordKeys->addChild(key);
	}
	dbRecord->addChild(recordKeys);
}

template <typename T>
void StorageManager<T>::createRecordValue(SPtr & dbRecord, Record<T> value)
{
	SPtr xmlRecord;
	xmlRecord = makeTaggedElement(this->VALUE_TAG);
	value.convertDataToXML(xmlRecord);
	dbRecord->addChild(xmlRecord);
}

//	Creates the db structure
template <typename T>
void StorageManager<T>::createDBStructure(DatabaseInfo dbInfo, XmlProcessing::XmlDocument & document) {

	SPtr pRoot = makeTaggedElement(this->XMLDATABASE_TAG);

	SPtr pDBMetadata = makeTaggedElement(this->METADATA_TAG);
	pRoot->addChild(pDBMetadata);
	//	Add database properties.
	SPtr dbProp = makeTaggedElement(this->DBNAME_TAG);
	SPtr dbPropValue = makeTextElement(dbInfo.DatabaseName);
	dbProp->addChild(dbPropValue);
	pDBMetadata->addChild(dbProp);
	dbProp = dbPropValue = nullptr;
	dbProp = makeTaggedElement(this->AUTOCOMMIT_TAG);
	dbPropValue = makeTextElement(Convert<int>::toString(dbInfo.AutoCommit));
	dbProp->addChild(dbPropValue);
	pDBMetadata->addChild(dbProp);

	document.docElement() = pRoot;
}

//	Thie method reads tries to open the .db.xml file and reads the contents to string.
template <typename T>
string StorageManager<T>::processDatabaseFile() {

	string xmlData = "";
	ifstream fileReader(this->databaseInfo->DatabaseName + this->FILEEXTENSION);
	if (fileReader.is_open())
	{
		string line = "";
		while (std::getline(fileReader, line))
		{
			xmlData += line + "\n";
		}
	}
	else
	{
		formatter->showMessage("Database \"" + this->databaseInfo->DatabaseName + "\"" + FILEEXTENSION + " not found. Terminating import.");
	}
	fileReader.close();
	return xmlData;
}

//	This method creates record objects from the XML data which is stored.
template <typename T>
unordered_map<string, Record<T>> StorageManager<T>::processXMLToObjects(string xmlFile) {
	unordered_map<string, Record<T>> cursor;
	DatabaseInfo dbInfo;
	if (xmlFile.length() > 0)
	{
		bool success = true;
		XmlDocument doc(xmlFile, XmlDocument::str);
		std::vector<SPtr> desc = doc.element(this->METADATA_TAG).select();  //descendents(this->XMLDATABASE_TAG).select();
		if (desc.size() == 1)
		{
			dbInfo.DatabaseName = trim(desc[0]->children()[0]->children()[0]->value());
			dbInfo.AutoCommit = Convert<int>::fromString(trim(desc[0]->children()[1]->children()[0]->value()));
		}
		std::vector<SPtr> desc1 = doc.element(this->RECORD_TAG).select();
		for (auto it = desc1.begin(); it != desc1.end(); ++it)
		{
			Record<T> recordObject;
			auto recordNode = *it;// ->children();
			const string objectId = trim(processMetadataNode(ObjectId, recordNode));
			recordObject.setItem(trim(processMetadataNode(Item, recordNode)).c_str());
			recordObject.setCategory(trim(processMetadataNode(Category, recordNode)).c_str());
			recordObject.setTime(trim(processMetadataNode(Time, recordNode)).c_str());
			recordObject.setDescription(trim(processMetadataNode(Description, recordNode)).c_str());
			auto keys = recordNode->children()[5]->children();
			for (auto keyIterator = keys.begin(); keyIterator != keys.end(); ++keyIterator)
			{
				recordObject.addRelationship(trim((*keyIterator)->children()[0]->value()));
			}
			createRecordDataObject(recordNode, recordObject);
			pair<string, Record<T>> recordInstance;
			recordInstance.first = objectId;
			recordInstance.second = recordObject;
			cursor.insert(recordInstance);
		}
	}
	return cursor;
}

//	This method returns the metadata value from the XML node and converts to string.
template<typename T>
string StorageManager<T>::processMetadataNode(MetadataType index, const SPtr & node) {
	if (node->children()[index]->children().size() > 0)
	{
		return node->children()[index]->children()[0]->value();
	}
	return "";
}

//	This method creates the templatized T data object.
template <typename T>
void StorageManager<T>::createRecordDataObject(SPtr & valuePointer, Record<T> & record)
{
	//	Read the objectid first
	record.setData(record.convertXMLToData(valuePointer->children()[6]));
}
