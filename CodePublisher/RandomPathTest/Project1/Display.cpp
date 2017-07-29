//////////////////////////////////////////////////////////////////////
//  DepAnalysisEngine.cpp - Dependency Analyzer engine				//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 2				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////


#include "Display.h"
#include <iostream>

using namespace std;

void Display::displayAnalysisResult(unordered_map<string, vector<string>> dependencies)
{
	cout << "---------------------------------------------------------------------\n";
	cout << "File" << "\t\tDependencies" << endl << endl;
	// << type defined in file and used in dependent files.
	for (auto iter = dependencies.begin(); iter != dependencies.end(); ++iter)
	{
		cout << iter->first << "\t\t" << iter->second.front() << endl;
		for (auto vecIter = iter->second.begin() + 1; vecIter != iter->second.end(); ++vecIter)
			cout << "\t\t\t" << *vecIter << endl;
		cout << "-----------------------------------------\n";
	}

}