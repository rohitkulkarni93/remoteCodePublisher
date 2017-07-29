#pragma once
//////////////////////////////////////////////////////////////////////
// NoSQLDB.h	-	The NoSQL database engine package				//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains the NoSQLDB type which is the DBEngine. This class is
* responsible for processing the different operations. This class uses the
* Core.StorageManager package for all the file related operations.
* It can perform the following operations:
*	-	Create Database.
*	-	Add / Update / Delete records.
*	-	Add / Update / Delete relationships.
*	-	Select records and display them.
*	-	Query framework is supports where condition on various fields.
*	-	Chaining queries (performing intersection and union of 2 queries).
*	-	Automatically trigger commit after configured write / update operations.
*
*
* Required Files:
* ---------------
*   - NoSQLDB.h
*
* Package Dependency:
* -------------------
*
*	- StorageManager.h - Package for managing storage of records.
*	- RecordStructure.h - Contains the record type.
*	- DatabaseStructure.h - Contains the DatabaseInfo type.
*
* Interfaces and Types:
* ---------------------
*	- NoSQLDB - This class is responsible for all the operations on
*				the records. It has various methods exposed to perform
*				different operations on the database.
*
* Maintenance History:
* --------------------
* ver 1.1 Feb 05 2017
* Developed the query framework to support querying the data.
* ver 1.0 Jan 2017
* Created the package with basic functionality.
*
* TODO:
* -----
* Enhance the query framework to support multiple select and
* to support "and" in where clause.
*	e.g. select ... ... where condition1 "and" condition2
* Split the code into different cpp files. Refer to this link.
* http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file/495056#495056
*/
#include <iostream>
#include <unordered_map>
#include <string>
#include "../Core.Datastructures/RecordStructure.h"
#include "../Core.StorageManager/StorageManager.h"
#include "../Core.Datastructures/DatabaseStructure.h"
#include "../Convert/Convert.h"
#include "IQueryChainer.h"
#include <string>
#include <regex>
#include <stack>
#include <set>
#include <ctime>

using namespace std;

template <typename T>
class NoSQLDB {

public:

	NoSQLDB(DatabaseInfo info) {
		this->dbInfo = new DatabaseInfo(info);
		this->formatter = new ConsoleFormatter();
		this->cursor = { };
		this->storageManager = new StorageManager<T>(*(this->dbInfo), this->formatter);
	}

	~NoSQLDB() {
		delete this->dbInfo;
		delete this->formatter;
		this->cursor.clear();
		delete this->storageManager;
	}

	//	This method imports a database from file and loads it in memory.
	void importDB() {
		this->cursor = this->storageManager->loadDatabase();
		this->formatter->showMessage("Import Processed.");
		this->formatter->showMessage("Fetched - " + Convert<int>::toString(this->cursor.size()) + " records.");
	}

	//	This method is used to execute select query on records.
	//		SelectObjectType - Choose what information of the record
	//						   needs to be displayed.
	//		WherePredicate	 - Choose which field of the record needs to be queried.
	//		MatchType		 - Choose if matching should be partial or exact.
	//		IQueryChainer	 - This parameter is used to chain 2 or more queries.

	IQueryChainer * select(
		SelectObjectType objectType,
		WherePredicate predicate,
		MatchType matchtype,
		vector<string> condition,
		IQueryChainer * query = nullptr) {
		auto keySet = processSelect(objectType, predicate, matchtype, condition, query);
		
		IQueryChainer * next = new IQueryChainer(keySet);
		return next;
	}

	//	Insert into database
	void addRecord(string key, Record<T> rec) {
		processInsert(key, rec);
	}

	//	Perform commit operation.
	void commit() {
		processCommit(CommitOperation::UserRequested);
	}

	//	Add a parent child relationship
	void addRelationship(string parent, string child) {
		processRelationshipUpdate(parent, child, 0);
	}

	//	Delete a parent child relationship
	void deleteRelationship(string parent, string child) {
		processRelationshipUpdate(parent, child, 1);
	}

	//	This method is used to update metadata of a record.
	//		key			 - Unique key of record
	//		MetadataType - Which metadata field needs to be updated.
	//		value		 - New value of the metadata field.
	void updateRecordMetadata(
		string key,
		MetadataType metadata,
		string value) {
		processMetadataUpdate(key, metadata, value);
	}

	//	This method is used to update value (data) of a record.
	//		key			 - Unique key of record
	//		value		 - New value of the data field.
	void updateRecordValue(string key, T value) {
		processValueUpdate(key, value);
	}

	//	This method is used to delete a record.
	void deleteRecord(string key) {
		processDelete(key);
	}
	
	//	Create a framework which can be provide a way to extend the nosqldb
	//	querying capabilities

	list<Record<T>> getRecords(IQueryChainer query)
	{
		list<Record<T>> records;
		vector<string> keys = query.getKeySet();
		for (auto key : keys) {
			records.push_back(cursor[key]);
		}
		return records;
	}

	//	Display records.

	void displayResult(IQueryChainer query, SelectObjectType display)
	{
		vector<string> keys = query.getKeySet();
		for (auto key : keys) {
			formatter->showRecord(cursor[key], display, key);
		}
	}

	string GetDbInfo()
	{
		return "Database Info\nName: " + dbInfo->DatabaseName + "\nAuto Commit: " + Convert<int>::toString(dbInfo->AutoCommit) + "\n";
	}

	void ChangeDatabaseAutoCommit(int autoCommitAfterOperations)
	{
		dbInfo->AutoCommit = autoCommitAfterOperations;
		this->writeOperations = 0;
	}

private:

	unordered_map<string, Record<T>> performUnion(
		unordered_map<string, Record<T>> & resultSet,
		vector<string> keySet)
	{
		vector<string> resultSetKeys, unionResult;
		unordered_map<string, Record<T>> unionres;
		for (auto iter = resultSet.begin(); iter != resultSet.end(); ++iter) {
			resultSetKeys.push_back(iter->first);
		}
		std::sort(resultSetKeys.begin(), resultSetKeys.end());
		std::sort(keySet.begin(), keySet.end());
		std::set_union(resultSetKeys.begin(), resultSetKeys.end(), keySet.begin(), keySet.end(),
			std::back_inserter(unionResult));
		for (auto iter = unionResult.begin(); iter != unionResult.end(); ++iter) {
			pair<string, Record<T>> keyValue;
			keyValue.first = *iter;
			keyValue.second = cursor[*iter];
			unionres.insert(keyValue);
		}
		return unionres;
	}

	unordered_map<string, Record<T>> performIntersection(
		unordered_map<string, Record<T>> & resultSet,
		vector<string> keySet)
	{
		vector<string> resultSetKeys, interesctionResult;
		unordered_map<string, Record<T>> intersection;
		for (auto iter = resultSet.begin(); iter != resultSet.end(); ++iter) {
			resultSetKeys.push_back(iter->first);
		}
		std::sort(resultSetKeys.begin(), resultSetKeys.end());
		std::sort(keySet.begin(), keySet.end());
		std::set_intersection(resultSetKeys.begin(), resultSetKeys.end(), keySet.begin(), keySet.end(),
			std::back_inserter(interesctionResult));
		for (auto iter = interesctionResult.begin(); iter != interesctionResult.end(); ++iter) {
			pair<string, Record<T>> keyValue;
			keyValue.first = *iter;
			keyValue.second = cursor[*iter];
			intersection.insert(keyValue);
		}
		return intersection;
	}

	vector<string> processSelect(
		SelectObjectType objtype,
		WherePredicate predicate,
		MatchType matchtype,
		vector<string> condition,
		IQueryChainer * query = nullptr) {
		//	formatter->showMessage("Select:");
		unordered_map<string, Record<T>> resultSet;
		vector<string> resultSetKeys;
		//	No where predicate.
		if (predicate != WherePredicate::WhereDefault) {
			resultSet = filterRecords(predicate, matchtype, condition);
		}
		else
			resultSet = this->cursor;
		//	Chain current query with previous
		if (query) {

			switch (query->getOperation()) {
			case 1:
				resultSet = performIntersection(resultSet, query->getKeySet());
				break;
			case 0:
				resultSet = performUnion(resultSet, query->getKeySet());
				break;
			}
		}
		for (auto iter = resultSet.begin(); iter != resultSet.end(); ++iter)
		{
			resultSetKeys.push_back(iter->first);
		}
		return resultSetKeys;
	}

	unordered_map<string, Record<T>> filterRecords(
		WherePredicate predicate,
		MatchType match,
		vector<string> condition)
	{
		unordered_map<string, Record<T>> resultSet;
		unordered_map<string, string> map;
		//	Check if requested query is based on key.
		//	Prepare data set to query based on metadata.
		for (auto it = cursor.begin(); it != cursor.end(); ++it)
		{
			pair<string, string> keyValue;
			keyValue.first = it->first;
			keyValue.second = getValueByWherePredicate(predicate, it->second, it->first);
			map.insert(keyValue);
		}
		//	Match pattern on the recordset and prepare result set
		bool matchFound;
		std::smatch smatch;
		for (auto iter = map.begin(); iter != map.end(); ++iter) {
			matchFound = false;
			for (auto stringIter = condition.begin(); 
				stringIter != condition.end(); ++stringIter) {
				//	Create regex based on condition.
				try {
					if (match == MatchType::PartialMatch) {
						string regexCondition = "[a-z]*" + *stringIter + "[a-z]*";
						std::regex regex(regexCondition);
						if (regex_search(iter->second, smatch, regex) && smatch.size() > 0) {
							matchFound = true;
						}
					}
					else {
						std::regex regex(*stringIter);
						if (regex_match(iter->second, smatch, regex) && smatch.size() > 0) {
							matchFound = true;
						}
					}
				}
				catch (exception ex) {}
				if (matchFound)
				{
					pair<string, Record<T>> rec;
					rec.first = iter->first;
					rec.second = cursor[iter->first];
					auto keyFound = resultSet.find(rec.first);
					if(keyFound == resultSet.end())
					{
						resultSet.insert(rec);
					}
				}
			}
		}
		return resultSet;
	}

	string getValueByWherePredicate(
		WherePredicate predicate,
		Record<T> record,
		string key) {
		switch (predicate)
		{
		case WherePredicate::WhereCategory:
			return record.getCategory();
		case WherePredicate::WhereDescription:
			return record.getDescription();
		case WherePredicate::WhereItem:
			return record.getItem();
		case WherePredicate::WhereTime:
			return record.getTime();
		case WherePredicate::WhereKey:
			return key;
		case WherePredicate::WhereValue:
			return record.getData();
		}
		return "";
	}

	void processRelationshipUpdate(string parent, string child, int addDelete) {
		formatter->showMessage("Update Relationship:");
		bool canUpdateRelationship = true;
		if (cursor.find(parent) == cursor.end()) {
			canUpdateRelationship = false;
			formatter->showException(OperationOutcome::KeyNotFound, parent);
		}
		else if (cursor.find(child) == cursor.end()) {
			canUpdateRelationship = false;
			formatter->showException(OperationOutcome::KeyNotFound, child);
		}
		else if (parent == child || cursor[child].isKeyPresent(parent)) {
			formatter->showException(OperationOutcome::InvalidRelationship, parent);
		}
		else {
			updateRelationship(parent, child, addDelete);
		}
	}

	void updateRelationship(string parent, string child, int addDelete) {
		if (addDelete == 0) {
			if (!cursor[parent].isKeyPresent(child)) {
				cursor[parent].addRelationship(child);
			}
			else {
				formatter->showException(OperationOutcome::RelationshipExist, child);
			}
		}
		else {
			if (cursor[parent].isKeyPresent(child)) {
				cursor[parent].deleteRelationship(child);
			}
			else {
				formatter->showException(OperationOutcome::KeyNotFound, child);
			}
		}
	}

	void processValueUpdate(string key, T value) {
		this->formatter->showMessage("Update record value: ");
		if (cursor.find(key) != cursor.end()) {
			cursor[key].setData(value);
			processPostWriteOperation();
			this->formatter->showMessage("Updated 1 row.");
		}
		else
		{
			this->formatter->showException(OperationOutcome::KeyNotFound, key);
		}
	}

	void processCommit(CommitOperation operation) {
		OperationOutcome outcome = this->storageManager->saveToFile(this->cursor);
		if (!(outcome == OperationOutcome::CommitProcessed))
		{
			this->formatter->showException(outcome, "Commit");
		}
		else
		{
			string message = (operation == CommitOperation::Scheduled) ? " (Scheduled)" : " (User Requested)";
			this->formatter->showMessage("Commit Processed. " + message);
			this->writeOperations = 0;
		}
	}

	void processPostWriteOperation() {
		writeOperations++;
		if (writeOperations == dbInfo->AutoCommit) {
			processCommit(CommitOperation::Scheduled);
		}
	}

	void processDelete(string key) {
		formatter->showMessage("Delete record:");
		if (cursor.find(key) != cursor.end())
		{
			cursor.erase(key);
			formatter->showMessage("Delete sucessful.");
			for (auto iter = cursor.begin(); iter != cursor.end(); ++iter)
			{
				if (iter->second.isKeyPresent(key))
				{
					iter->second.deleteRelationship(key);
				}
			}
		}
		else
		{
			formatter->showException(OperationOutcome::KeyNotFound, key);
		}
	}

	void defaultDateTime(Record<T> & t) {
		time_t now = time(0);
	//	char * currentDateTime = new char[100];
	//	ctime_s(currentDateTime, sizeof(&currentDateTime), &now);
	//	t.setTime(currentDateTime);
	}

	void processInsert(string key, Record<T> t) {
		string faultingIdentifier = "";
		OperationOutcome outcome = evaluateInsertConstraints(key, t, faultingIdentifier);
		defaultDateTime(t);
		if (outcome == OperationOutcome::Success)
		{
			//	Process insert.
			pair<string, Record<T>> recData;
			recData.first = key;
			recData.second = t;
			cursor.insert(recData);
			processPostWriteOperation();
			//this->formatter->showMessage("Inserted 1 row.");
		}
		else
		{
			this->formatter->showException(outcome, faultingIdentifier);
		}
	}

	void updateMetadata(
		string key,
		MetadataType metadata,
		string value) {
		switch (metadata)
		{
		case MetadataType::Category:
			this->cursor[key].setCategory(value.c_str());
			break;
		case MetadataType::Description:
			this->cursor[key].setDescription(value.c_str());
			break;
		case MetadataType::Item:
			this->cursor[key].setItem(value.c_str());
			break;
		case MetadataType::Time:
			this->cursor[key].setTime(value.c_str());
			break;
		default:
			break;
		}
	}

	void processMetadataUpdate(string key, MetadataType metadata, string value) {
		this->formatter->showMessage("Update Metadata: " + getMetadataCode(metadata));
		if (cursor.find(key) != cursor.end()) {
			if (metadata == MetadataType::Item && key == cursor[key].getItem())
			{
				this->formatter->showException(OperationOutcome::PrimaryKeyEdit, key);
			}
			else
			{
				updateMetadata(key, metadata, value);
				processPostWriteOperation();
				this->formatter->showMessage("Updated 1 row.");
			}
		}
		else {
			this->formatter->showException(OperationOutcome::KeyNotFound, key);
		}
	}

	OperationOutcome evaluateInsertConstraints(
		string key,
		Record<T> rec,
		string & identifier) {
		if (cursor.size() > 0 && cursor.find(key) != cursor.end())
		{
			identifier = key;
			return OperationOutcome::DuplicateKey;
		}
		for each (auto var in rec.getChildKeys())
		{
			if (cursor.find(var) == cursor.end())
			{
				identifier = var;
				return OperationOutcome::KeyNotFound;
			}
		}
		return OperationOutcome::Success;
	}

	unordered_map<string, Record<T>> cursor;	//	In-memory storage
	DatabaseInfo * dbInfo;
	StorageManager<T> * storageManager;			//	StorageManager instance
	IOutputFormatter * formatter;				//	Formatter object for outputing data
	int writeOperations = 0;
};

