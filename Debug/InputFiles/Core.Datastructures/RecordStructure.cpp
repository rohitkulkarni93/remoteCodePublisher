#pragma once
//////////////////////////////////////////////////////////////////////
// RecordStructure.cpp - Implementation of the methods in template	//
//						 specialized Record<string> type			//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "RecordStructure.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "StrHelper.h"

using namespace XmlProcessing;

//	Template specialized class in strings which provides 
//	the conversion logic required for the database engine.
void Record<std::string>::convertDataToXML(SPtr & xmlDocument) {
	SPtr dataNode = makeTaggedElement("datastr");
	dataNode->addChild(makeTextElement(this->getData()));
	xmlDocument->addChild(dataNode);
}

//	Convert XML to data.
string Record<std::string>::convertXMLToData(SPtr xmlDocument) {
	SPtr dataNode = xmlDocument->children()[0];
	if (dataNode->children().size() > 0)
	{
		return trim(dataNode->children()[0]->value());
	}
	return "";
}

//	IRecordFormatter interface implementation.
//	The record formats data depending the selection criterion.
string Record<std::string>::formatRecord(SelectObjectType objectType, string keyValue)
{
	string formattedRecordObject = "Key: " + keyValue + ", ";
	switch (objectType)
	{
	case SelectObjectType::SelObject:
		formattedRecordObject += "Item - " + getItem() + ", Category - " + getCategory() + ", Description - "
			+ getDescription() + ", Data - " + getData() + ", Time - " + getTime();
		break;
	case SelectObjectType::SelItem:
		formattedRecordObject += "Item - " + getItem();
		break;
	case SelectObjectType::SelDescription:
		formattedRecordObject += "Description - " + getDescription();
		break;
	case SelectObjectType::SelKey:
		break;
	case SelectObjectType::SelChildKey:
		formattedRecordObject += "Children - ";
		for (auto it = this->getChildKeys().begin(); it != this->getChildKeys().end(); ++it)
			formattedRecordObject += (*it + " ");
		break;
	case SelectObjectType::SelTime:
		formattedRecordObject += "Time - " + getTime();
		break;
	case SelectObjectType::SelValue:
		formattedRecordObject += "Value - " + getData();
		break;
	default:
		formattedRecordObject += "Item - " + getItem() + ", Categoty - " + getCategory() + ", Description - "
			+ getDescription() + ", Data - " + getData() + ", Time - " + getTime() + "\n";
		break;
	}
	return formattedRecordObject;
}
//	<------------------------- Test ----------------------------->
//	Demonstrates the basic use of the record class.
//	Uncomment the following code and run it.
//int main() {
//	
//	int a;
//	Record<string> rec;
//	rec.setCategory("Hello");
//	rec.setItem("Workd");
//	rec.setDescription("First assignment");
//	rec.setTime("CurrentTime");
//	rec.setData("This is some random data");
//	rec.addRelationship("Hello");
//	cout << rec.formatRecord(SelectObjectType::SelObject, "Key") << endl;
//	cout << rec.formatRecord(SelectObjectType::SelDescription, "Key") << endl;
//	cin >> a;
//}