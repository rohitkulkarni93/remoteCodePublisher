#pragma once
//////////////////////////////////////////////////////////////////////
// DatabaseStructure.h - Contains database structure.				//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains the database structure. This type describes
* the information regarding the database. All the settings related to the
* database can be added to this structure.
* e.g.	- logging,
*		- settings
*
* Required Files:
* ---------------
*   - DatabaseStructure.h
*
* Package Dependency:
* -------------------
*	- This package is one of the core components should compile before any other.
*
* Interfaces and Types:
* ---------------------
* ver 1.0 (Jan 2017)
*		- DatabaseInfo : Currently this type has members to store database name and
*						 commit after operations.
*
* Maintenance History:
* --------------------
* ver 1.0 - Jan 2017
* Created the package.
*
* TODO:
* -----
*/

#include <iostream>

using namespace std;

// This type is used for storing information related to database.
// Any database settings which need to be persisted can be added here.
// 

class DatabaseInfo {
public:
	string DatabaseName;
	int AutoCommit;
};