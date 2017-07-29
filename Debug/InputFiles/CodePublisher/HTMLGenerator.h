#pragma once
//////////////////////////////////////////////////////////////////////
// HTMLGenerator - Package is used for generating html, js and css	//
//				   files											//
// Ver 1.0															//
// Application: Dependency based Code Publisher						//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package generates html files, css files and javascript files.
*
* Required Files:
* ---------------
*   - HTMLGenerator.h
*	- HTMLGenerator.cpp
*
* Package Dependency:
* -------------------
*	This package doesn't depend on any package.
*
* Public Interface:
* ---------------------
*	HTMLGenerator generator;
	//	Create html page with dependencies
*	createHtmlPage(string, string, unordered_map<string, vector<string>>);
*	generateJavascriptFile();	//	Generate javascript file
*	generateCSSFile();			//	Generate css file
*
* Maintenance History:
* --------------------
* ver 1.0 - April 2017
* Created the package and added the basic interfaces and classes.
*
*/

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

//	Simple structure for configuration.
struct Config
{
	string cssFileName;
	string jsFileName;
};

class HtmlGenerator
{
public:
	void operator()(Config);
	HtmlGenerator();
	HtmlGenerator(Config);
	string createHtmlPage(string fileName, string data, unordered_map<string, vector<string>> dependencies);
	void generateJavascriptFile();
	string generateCSSFile();
private:
	bool shouldExpandCollapse(string & data, int index, int prevPosition);
	string addScriptLink();
	string injectJavascriptFunctionality(string data);
	string createTag(const string tag, bool open = true, string attributeInfo = "");
	string addDependencies(string fileName, unordered_map<string, vector<string>> dependencies);
	Config config;
};