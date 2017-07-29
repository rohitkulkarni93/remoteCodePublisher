#pragma once
//////////////////////////////////////////////////////////////////////
//  TypeTable.h -	package that contains the structures to store	//
//					the type table									//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 2				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
------------------
The package contains structure to hold the type information which
the TypeAnalyzer extracts from the abstract syntax tree.
It uses the NoSQLDB to store the type information and persist the
type table into XML.

	Type -	used to hold the information of types and different elements
			in the type table.

	Typetable -	table stores type structure in nosql database and
				offers an interface for dependency analysis to get
				the required information.

Required files:
---------------
TypeTable.h
TypeTable.cpp

Maintenance History:
====================
ver 1.0
First release.
*/


#include <string>
#include <vector>
#include <unordered_map>
#include "../Core.DBEngine/NoSQLDB.h"
#include "../Core.Datastructures/CoreInterfaces.h"

using namespace std;

struct Type
{
	std::string typeName;
	std::string typeInfo;
	std::string packageName;
	std::string pathName;
	std::string fullyQualifiedName;
};

class TypeTable {

public:
	TypeTable();
	~TypeTable();
	void printTypeTable();
	void addTypeToTable(Type);
	list<Type> lookup(string, string);
	void commit();
private:
	//	Mapping:
	//	typename = data
	//	typeinfo = category
	//	packageName = description
	//	pathName = item

	DatabaseInfo * typeTableInfo;
	NoSQLDB<string> * typeTable;
	int counter = 0;
};



