#pragma once
//////////////////////////////////////////////////////////////////////
// CodePublisher - Package publishes static code files				//
// Ver 1.0															//
// Application: Dependency based Code Publisher						//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package processes the path recursively to find files matching the 
* pattern and creates html pages.
*
* Required Files:
* ---------------
*   - CodePublisher.h
*	- CodePublisher.cpp
*
* Package Dependency:
* -------------------
*	HtmlGenerator - This package uses the HTMLGenerator package to generate html files.
*	Config - Not a direct dependency but can be used to configure Publisher.
*
* Public Interface:
* ---------------------
*	CodePublisher codePublisher;					//	
*	importDependencies(string);						//	Import dependencies by dependency analyzer.
*	publishCode(string, string, vector<string>);	//	publish code files
*	configure(Config)								//	Configure the parser
*
* Maintenance History:
* --------------------
* ver 1.0 - April 2017
* Created the package and added the basic interfaces and classes.
*
*/
#include <iostream>
#include <string>
#include <unordered_map>
#include "../FileSystem/FileSystem.h"
#include "../XmlDocument/XmlDocument.h"
#include "HTMLGenerator.h"


using namespace std;

//	Tokenize file and replace markups
//	get input from dependency analyzer
//	generate hyperlinks for dependencies
//	generate css
//	generate js

class CodePublisher
{
public:
	
	CodePublisher();
	void publishCode(string path, string outputPath, vector<string> pattern);
	unordered_map<string, vector<string>> importDependenciesFromFile(string dependencyfileName);
	void importDependencies(const unordered_map<string, vector<string>> & dependencies);
	void configure(Config config);
	void publishWebpage(string pagename);
private:
	string publishPath;
	void processDirectory(string path, vector<string> patterns);
	string getDirectoryName(string base, string currentDirectory);
	void processFiles(vector<string> files);
	void generateHtmlFile(string fileName, string convertedSourceCode);
	string makeReplacements(string file);
	unordered_map<string, vector<string>> dependencies;
	HtmlGenerator generator;
};