#pragma once
//////////////////////////////////////////////////////////////////////
// ConsoleFormatter.cpp - Contains implementation of methods.		//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////

//  This class provides implementation for the methods in 
//	IOutputFormatter interface. This type can be injected in the
//	DBEngine to get output all the messages to console.

#include <iostream>
#include "CoreInterfaces.h"
#include "RecordStructure.h"

using namespace std;

void ConsoleFormatter::showMessage(string message) {
	cout << message << endl;
}

void ConsoleFormatter::showException(OperationOutcome outcome, string identifier) {
	cout << getOperationOutcomeCode(outcome) << ": \"" << identifier << "\" " << getOperationOutcomeMessage(outcome) << endl;
}

void ConsoleFormatter::showRecord(IRecordFormatter & rec, SelectObjectType objectType, string key) {
	cout << rec.formatRecord(objectType, key) << endl;
}

//	<------------------------- Test ----------------------------->
//	Demonstrates the basic use of the ConsoleFormatter class.
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
//	
//	ConsoleFormatter cf;
//	cf.showException(OperationOutcome::KeyNotFound, "Key1");
//	cf.showMessage("Simple message");
//	cf.showRecord(rec, SelectObjectType::SelObject, "KEY2");
//	cin >> a;
//}