#pragma once
//////////////////////////////////////////////////////////////////////
//  DepAnalysisEnging.h - the dependency analyzer					//
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
This package is used to perform dependency analysis on the selected
files. It uses the typetable build in the previous phase to look up
for matching tokens and analyze dependencies.

startDependencyAnalysis(const char * path)	-	This method starts 
				the dependency analysis on the specified path. It 
				requires the type table to be imported before actually
				starting the analysis.

importTypeTable()	-	This method is used to set the typetable for
				the dep analyzer.

exportAnalysisResult()	-	This method exports the analysis result

exportAnalysisResultToFile()	-	This method writes the analysis
				result to the specified file and path.

Required files:
---------------
DepAnalysisEngine.h
DepAnalysisEngine.cpp

Maintenance History:
====================
ver 1.0
First release.
*/


#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <list>
#include "TypeTable.h"
#include "../Tokenizer/Tokenizer.h"

using namespace std;

class DependencyAnalysisEngine
{
public:
	DependencyAnalysisEngine();
	~DependencyAnalysisEngine();
	/// This method triggers the dependency analysis on the set of
	///	files at that path.
	void startDependencyAnalysis(const char * path);
	void importTypeTable(TypeTable & table);
	unordered_map<string, vector<string>> exportAnalysisResult();
	void exportAnalysisResultToFile(string fileName, string path);
	unordered_map<string, vector<string>> readAnalysisResultFromFile(string fileName);
private:
	//	Member functions.
	void processDirectory(string path);
	string getDirectoryName(string base, string currentDirectory);
	void processFiles(vector<string> files);
	void tokenizeAndProcessFile(string fileName);
	void processTokens(queue<string> tokenQueue, string fileName);	
	void processDependency(string token, string fileName);
	void processDependency(Type * type, string fileName);
	void processContext(queue<string> tokenQueue);
	//	Data Members.
	unordered_map<string, vector<string>> * dependencies;
	TypeTable * typeTable;
	list<string> * namespaceContext;
};