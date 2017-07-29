//////////////////////////////////////////////////////////////////////
//  TypeTable.cpp - Type table package								//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 2				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////


#include "TypeTable.h"
#include "../Core.DBEngine/IQueryChainer.h"
#include "../Convert/Convert.h"

TypeTable::TypeTable() {
	this->typeTableInfo = new DatabaseInfo();
	this->typeTableInfo->AutoCommit = -1;
	this->typeTableInfo->DatabaseName = "TypeTable";
	this->typeTable = new NoSQLDB<string>(*this->typeTableInfo);
}

//	Free memory and make pointers null.
TypeTable::~TypeTable() {
	this->typeTableInfo= nullptr;
	this->typeTable = nullptr;
}

//	Add to type table
void TypeTable::addTypeToTable(Type type)
{
	string ctr = Convert<int>::toString(counter++);
	Record<string> record;
	record.setCategory(type.typeInfo.c_str());
	record.setData(type.typeName.c_str());
	record.setItem(type.pathName.c_str());
	record.setDescription(type.packageName.c_str());
	record.setTime(type.fullyQualifiedName.c_str());
	this->typeTable->addRecord(ctr, record);
}

//	Save the typetable to file. Use nosql commit functionality to achieve this.
void TypeTable::commit() {
	this->typeTable->commit();
}

//	Look up token and namespace.
list<Type> TypeTable::lookup(string token, string namespaceLike = "")
{
	list<Type> typeList;
	bool scanToken = true;
	if (token.length() < 5) {
		for (int i = 0, len = token.length(); i < 5, len > 0; i++, len--) {
			if (!((token[i] > 64 && token[i] < 91 )|| (token[i] > 96 && token[i] < 123) || (token[i] > 47 && token[i] < 58 ))) {
				scanToken = false;
				break;
			}
		}
	}
	if (!scanToken)
		return typeList;
	
	vector<string> vec;
	vec.push_back(token);
	IQueryChainer *query = typeTable->select(SelKey, WherePredicate::WhereValue, MatchType::ExactMatch, vec, nullptr);
	if (namespaceLike != "")
	{
		vec.pop_back();
		vec.push_back(namespaceLike);
		query = typeTable->select(SelKey, WherePredicate::WhereTime, MatchType::PartialMatch, vec, query->chainIntersection());
		//	Perform intersection of query with namespace is used.
	}
	//	get records.
	list<Record<string>> records = typeTable->getRecords(*query);
	
	if (records.size() > 0)
	{
		for (auto rec : records) {
			Type type;
			type.typeInfo = rec.getCategory();
			type.typeName = rec.getData();
			type.pathName = rec.getItem();
			type.packageName = rec.getDescription();
			type.fullyQualifiedName = rec.getTime();
			typeList.push_back(type); 
		}
	}
	if (typeList.size() > 0) {
		cout << "";
	}
	return typeList;
}

//	Print the typetable.
void TypeTable::printTypeTable()
{
	cout << "\n\nPrinting Type table:" <<endl;
	auto query = this->typeTable->select(
		SelectObjectType::SelObject, 
		WherePredicate::WhereDefault, 
		MatchType::ExactMatch, 
		vector<string>());
	typeTable->displayResult(*query, SelValue);
	cout << endl << endl;

}