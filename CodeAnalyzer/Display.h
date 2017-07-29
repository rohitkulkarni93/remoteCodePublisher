#pragma once
//////////////////////////////////////////////////////////////////////
//  Display.h -		package handles the displaying of the dependency//
//					analysis result									//
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
This package handles the displaying of result of the dependency analyzer.
It has one method which takes a map with dependencies, and displays it.
Note: For typetable printing, since the data is stored in the nosql database,
the Formatter.h package of the nosql db manages its display.

Required files:
---------------
Display.h
Display.cpp

Maintenance History:
====================
ver 1.0
First release.
*/

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Display {
public:
	void displayAnalysisResult(unordered_map<string, vector<string>> dependencies);
private:

};