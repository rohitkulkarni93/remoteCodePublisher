//////////////////////////////////////////////////////////////////////
// NoSQLDB.cpp	-	Basically an emply file.						//
// Ver 1.0															//
// Application: NoSQL Database										//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////

//	If template class functions are moved in here and resolved using
//	the scope resolutor, LNK2019 error shows up. Hence all the functions
//	are in the header file NoSQLDB.h

#include <iostream>
#include <unordered_map>
#include <string>
#include "NoSQLDB.h"
#include "IQueryChainer.h"
#include "../Core.Datastructures/RecordStructure.h"
#include "../Core.StorageManager/StorageManager.h"
#include "../Core.Datastructures/DatabaseStructure.h"
#include "../Convert/Convert.h"
#include "IQueryChainer.h"

//	<------------------------- Test ----------------------------->
//	Demonstrates the basic use of the NoSQLDB class.
//	Uncomment the following code and run it.
//	This class also includes the testing of all the methods in the 
//	StorageManager class
//
//int main()
//{
//	DatabaseInfo databaseInfo;
//	databaseInfo.DatabaseName = "smartphones1";
//	databaseInfo.AutoCommit = 2;
//
//	NoSQLDB<string> db(databaseInfo);
//	cout << "Importing database - " << databaseInfo.DatabaseName << endl;
//	db.importDB();
//	db.select(SelectObjectType::SelObject, WherePredicate::WhereDefault, MatchType::ExactMatch, "", nullptr);
//	cout << endl << "Requirement 3. Adding a new record. " << endl;
//
//	Record<string> record;
//	record.setItem("iPhone");
//	record.setCategory("Phones");
//	record.setDescription("The iPhone 7 Plus");
//	record.setData("This is the best smartphone by apple.");
//	cout << "For this record, item name is used as key. hence item name cant be modified." << endl;
//
//	db.addRecord(record.getItem(), record);
//
//	cout << endl << endl << "Requirement 4 - Trying to change the key of the record." << endl;
//	db.updateRecordMetadata("iPhone", MetadataType::Item, "iPhone 8");
//
//
//	Record<string> recordTwo;
//	recordTwo.setItem("iPad");
//	recordTwo.setCategory("SmartPhones");
//	recordTwo.setDescription("The iPhone 7 Plus");
//	recordTwo.setData("This is the best tablet by apple.");
//	recordTwo.setTime("10:10");
//	db.addRecord("iPaddd", recordTwo);
//
//	cout << endl << endl << "Requirement 4 - Performing various updates on the newly added record." << endl;
//	//	
//	db.updateRecordMetadata("iPaddd", MetadataType::Category, "Tablets");
//	db.updateRecordMetadata("iPaddd", MetadataType::Description, "The iPad Pro");
//	db.updateRecordValue("iPaddd", "The iPad pro is very powerful.");
//	db.updateRecordMetadata("iPaddd", MetadataType::Time, "10:20");
//
//	cout << endl << endl << "Requirement 4 - Adding and removing relationship" << endl;
//	db.addRelationship("iPaddd", "iPhone");
//	cout << endl << endl << "Displaying the records children." << endl;
//
//	Record<string> recordThree;
//	recordThree.setItem("iPad1");
//	recordThree.setCategory("SmartPhones");
//	recordThree.setDescription("The iPhone 7 Plus");
//	recordThree.setData("This is the best tablet by apple.");
//	recordThree.setTime("10:10");
//
//	db.addRecord(recordThree.getItem(), recordThree);
//	recordThree.setCategory("Tablets");
//	db.addRecord("iPad2", recordThree);
//	db.addRecord("iPad3", recordThree);
//	db.addRecord("iPad4", recordThree);
//	recordThree.setCategory("Macbooks");
//	db.addRecord("iPad5", recordThree);
//	db.addRecord("iPad6", recordThree);
//	db.addRecord(recordThree.getItem(), recordThree);
//
//	db.addRelationship("iPad2", "iPad2");
//
//	IQueryChainer * query1 =
//		db.select(SelectObjectType::SelObject,
//			WherePredicate::WhereKey,
//			MatchType::PartialMatch,
//			"iPad");
//	IQueryChainer * query2 =
//		db.select(SelectObjectType::SelItem,
//			WherePredicate::WhereCategory,
//			MatchType::ExactMatch,
//			"SmartPhones", query1->chainUnion());
//
//	cout << endl << endl << "Requirement 4 - Trying to add a record as child which doesnt exist." << endl;
//	db.addRelationship("iPaddd", "Macbook");
//	cout << endl << endl << "Displaying the records children." << endl;
//	db.select(SelectObjectType::SelChildKey, WherePredicate::WhereKey, MatchType::ExactMatch, "iPaddd");
//
//	cout << endl << endl << "Requirement 5 - Performing commit operation.";
//	db.commit();
//}
