#pragma once
//////////////////////////////////////////////////////////////////////
// GlobalConstants.h - As the name suggests, it contains global		//
//					   constants and enumerations.					//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Information:
* -------------------
* This package contains is created with the intension to have all the 
* constants and enumerations in a single place. Being in a Core library
* it is easily available across the different components in the application.
* 
* 
* Required Files:
* ---------------
*   - GlobalConstants.h
*
* Package Dependency:
* -------------------
*	- This package is one of the core components should compile before any other.
*
* Interfaces and Types:
* ---------------------
* ver 1.0 (Jan 2017)
*		
*	- "SPtr" : Alias for "std::shared_ptr<XmlProcessing::AbstractXmlElement>"
*			   this alias is used in different components.
*
*	- OperationOutcome : This enum is used to define the different outcomes of a 
*						 DML query.
*
*	- CommitOperation	: Different commit operations.
*
*	- MetadataType		: This enum is used by the DBEngine component to identify different
*						  types of metadata information in a record.
*
*	- SelectObjectType	: This enum is used by the DBEngine to different select query
*						  outcomes.
*
*	- WherePredicate	: The WherePredicate enumeration contains the different fields
*						  on which where clause can be executed.
*
*	- MatchType			: The match type is used by the DBEngine for matching the query
*						  with regular expressions.
*
* Maintenance History:
* --------------------
* ver 1.0 - Jan 2017
* Created the package.
*
* TODO:
* -----
* Move the static functions into a lightweight injectable object.
* Use namespaces for the Enumerations.
*/

#include <iostream>
#include "../XmlDocument/XmlDocument.h"

//	Definition of SPtr only once in this header file.
//	This header file can be used across the application.
//
//	TODO:	- Get rid of the static methods and find a better solution.
//			- Add the enums to a namespace rather than keeping them global.

using namespace std;
using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

//	This enum defines the different outcomes of a DML query.
enum OperationOutcome {
	Success,
	Failure,
	DuplicateKey,
	KeyNotFound,
	PrimaryKeyEdit,
	RelationshipExist,
	CommitProcessed,
	InvalidRelationship
};

//	This enum defines the different commit operations types.
enum CommitOperation {
	UserRequested,
	Scheduled
};

//	This enum is used by the StorageManager to identify metadata
//	type while creating record objects from file and while persisting them
//	back to files.
enum MetadataType {

	ObjectId = 0,
	Item = 1,
	Category = 2,
	Time = 3,
	Description = 4,
	Keys = 5
};

//	This enum is used by the DBEngine to perform selection.
enum SelectObjectType {
	SelObject,			//	Default entire object
	SelItem,			//	For finding item
	SelCategory,		//	For selecting category
	SelTime,			//	For selecting time (difference)
	SelDescription,		//	For description
	SelValue,			//	Value of instance
	SelChildKey,		//	Selecting Children
	SelKey				//	Key	
};

//	This enum is used by the DBEngine to process where clause.
enum WherePredicate {
	WhereDefault,
	WhereItem,
	WhereCategory,
	WhereTime,
	WhereDescription,
	WhereValue,
	WhereChildKey,
	WhereKey
};

//	This enum is used by DBEngine while regular expression matching.
enum MatchType {
	ExactMatch,
	PartialMatch,
};

//	This method is used to get the text description related to the 
//	OperationOutcome enumeration.
string static getOperationOutcomeMessage(OperationOutcome outcome) {
	switch (outcome)
	{
	case OperationOutcome::DuplicateKey:
		return "Unique constraint violated.";
	case OperationOutcome::KeyNotFound:
		return "Key not found.";
	case OperationOutcome::CommitProcessed:
		return "Commit Processed.";
	case OperationOutcome::PrimaryKeyEdit:
		return "Primary Key cant be edited.";
	case OperationOutcome::RelationshipExist:
		return "Parent child relationship exists.";
	case OperationOutcome::InvalidRelationship:
		return "Recursive relationship not supported.";
	default:
		return "";
		break;
	}
}

//	This method is used to translate the OperationOutcome enum value
//	to string
static string getOperationOutcomeCode(OperationOutcome outcome) {
	switch (outcome)
	{
	case OperationOutcome::DuplicateKey:
		return "DuplicateKey";
	case OperationOutcome::KeyNotFound:
		return "KeyNotFound";
	case OperationOutcome::CommitProcessed:
		return "CommitProcessed";
	case OperationOutcome::PrimaryKeyEdit:
		return "PrimaryKeyEdit";
	case OperationOutcome::RelationshipExist:
		return "RelationshipExist";
	case OperationOutcome::InvalidRelationship:
		return "InvalidRelationship";
	default:
		return "";
		break;
	}
}

static string getMetadataCode(MetadataType code) {
	switch (code)
	{
	case ObjectId:
		return "OBJECTID";
	case Item:
		return "ITEM";
	case Category:
		return "CATEGORY";
	case Time:
		return "TIME";
	case Description:
		return "DESCRIPTION";
	case Keys:
		return "CHILDREN";
	default:
		return "";
		break;
	}
}