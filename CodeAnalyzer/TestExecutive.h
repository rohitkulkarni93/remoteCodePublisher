#pragma once
//////////////////////////////////////////////////////////////////////
//  TestExecutive.h - Test executive package for Dependency Analyzer//
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
This package is aimed at demonstrating all requirements for the dependency
analyzer project.

Required files:
---------------
TestExecutive.h

Maintenance History:
====================
ver 1.0
First release.
*/

#include <iostream>
using namespace std;

class TestExecutive
{
public:

	void demoAllReqs() {
		information();
		demoReq1();
		demoReq2();
		demoReq3();
		demoReq4();
		demoReq5();
		demoReq6();
		demoReq7();
		demoReq8();
		demoReq9();
	}

private:
	void req(int i) {
		cout << endl << "REQUIREMENT " << i;
		cout << endl << "------------------------------------------------------------";
		cout << endl;
	}

	void information() {
		cout << "Rohit Vidyadhar Kulkarni" << endl;
		cout << "SUID - 618809126"<< endl << endl;
		cout << "Project #2 - Type-based Dependency Analysis" << endl;
		cout << "Object oriented design class CSE 687" << endl;
		cout << endl << "------------------------------------------------------------";
	}

	void demoReq1() {
		req(1);
		cout	<<	"This project has been developed using Visual Studio 2015 \n"
				<<	"and its C++ Windows Console Projects"
				<<	endl;
	}
	void demoReq2() {
		req(2);
		cout	<<	"The C++ standard library's streams are used for all I/O and \n"
				<<	"new and delete for all heap-based memory management."
				<<	endl;
	}
	void demoReq3() {
		req(3);
		cout	<<	"C++ packages as described in the Purpose section. \n"
				<<	"Following packages are provided\n"
				<<	" 1) TypeTable\n"
				<< "  2) TypeAnalysis\n"
				<< "  3) DependencyAnalysis\n"
				<< "  4) NoSQLDB\n"
				<< "  5) Formatter\n (Does the purpose of display)"
				<< "  6) Test Executive\n"
				<<	endl;
	}
	void demoReq4() {
		req(4);
		cout	<<	"Type analysis package is provided which performs type analysis."
				<<	endl;
	}
	void demoReq5() {
		req(5);
		cout	<<	"Dependency analysis package is provided which performs dependency \n"
				<<	"analysis on the files using the type table."
				<<	endl;
	}
	void demoReq6() {
		req(6);
		cout	<<	"This req has not been implemented."
				<<	endl;
	}
	void demoReq7() {
		req(7);
		cout	<<	"The dependency analysis results is written in XML format, to a specified.\n"
				<<	"The type table is also saved to a XML file."
				<<	endl;
	}
	void demoReq8() {
		req(8);
		cout	<<	"For this requirement, the first 2 points are already covered by professors code.\n"
				<<	"For the third point, i.e. xml file specification, a new command line argument is added.\n"
				<<	"	/s [FILENAME] : this is to specify the file to save the result to.\n"
				<<	"If no file is specified, data is exported to Export.xml by default."
				<<	endl;
	}
	void demoReq9() {
		req(9);
		cout	<<	"This test suite executes all the requirements have been demonstrated"
				<< endl;
	}
};