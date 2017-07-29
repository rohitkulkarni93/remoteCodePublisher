#pragma once
//////////////////////////////////////////////////////////////////////
// IQueryChainer.h	-	This package is used for chaining select	//
//						queries										//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used to perform intersect and union operations on any
* number of select queries. Resultset of previous query can be used to 
* perform intersection and union with the current query.
* Operations:
*	-	chainIntersection : Tells the DBEngine to perform an "intersection" 
*							on the current resultset and the executing query
*	-	chainUnion :		Tells the DBEngine to perform a "Union" operation
*							on the current resultset and the executing query
*
* Required Files:
* ---------------
*   - IQueryChainer.h
*
* Interfaces and Types:
* ---------------------
*	- IQueryChainer - This class is used to perform intersection and union
*					  on two or more queries.
*
* Maintenance History:
* --------------------
* ver 1.0 Feb 05 2017
* Developed the query framework to support querying the data.
*
* TODO:
* -----
*/
#include<vector>
#include<string>
#include<list>

using namespace std;

class IQueryChainer {
public:
	IQueryChainer(vector<string> resultSet) {
		keyset = resultSet;
	}

	//	This method returns the keys from the current query.
	vector<string> getKeySet() const {
		return keyset;
	}

	//	This method helps perform union on 2 queries.
	IQueryChainer * chainUnion() {
		this->operation = 0;
		return this;
	}

	//	This method helps perform intersection on 2 queries.
	IQueryChainer * chainIntersection() {
		this->operation = 1;
		return this;
	}

	int getKeysetCount() const {
		return keyset.size();
	}

	int getOperation() const {
		return operation;
	}

	~IQueryChainer() {

	}

private:
	vector<string> keyset;
	int operation = -1;
};