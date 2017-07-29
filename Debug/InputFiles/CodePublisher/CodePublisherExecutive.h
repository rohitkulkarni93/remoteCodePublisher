#pragma once
//////////////////////////////////////////////////////////////////////
//  CodePublisherExecutive - Executive package for Code Publisher	//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 3				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
------------------
This package is aimed at demonstrating all requirements for the code
publisher project.

Required files:
---------------
CodePublisherExecutive.h

Maintenance History:
====================
ver 1.0
First release.
*/

#include <iostream>
using namespace std;

class CodePublisherRequirementDemo
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
		demoReq10();
	}

private:
	void req(int i) {
		cout << endl << "REQUIREMENT " << i;
		cout << endl << "------------------------------------------------------------";
		cout << endl;
	}

	void information() {
		cout << "Rohit Vidyadhar Kulkarni" << endl;
		cout << "SUID - 618809126" << endl << endl;
		cout << "Project #3- Type-based Code Publisher" << endl;
		cout << "Object oriented design class CSE 687" << endl;
		cout << endl << "------------------------------------------------------------";
	}

	void demoReq1() {
		req(1);
		cout << "This project has been developed using Visual Studio 2015 \n"
			<< "and its C++ Windows Console Projects"
			<< endl;
	}
	void demoReq2() {
		req(2);
		cout << "The C++ standard library's streams are used for all I/O and \n"
			<< "new and delete for all heap-based memory management."
			<< endl;
	}
	void demoReq3() {
		req(3);
		cout << "Provides a Publisher program that provides for "
			<< "creation of web pages each of which captures the content of a "
			<< "single C++ source code file, e.g., *.h or *.cpp."
			<< endl;
	}
	void demoReq4() {
		req(4);
		cout << "Provides the facility to expand or collapse class bodies, "
			<< "methods, and global functions using JavaScript and CSS properties."
			<< endl;
	}
	void demoReq5() {
		req(5);
		cout << "Provides a CSS style sheet that the Publisher uses to "
			<< "style its generated pages and a JavaScript file that provides "
			<< "functionality to hide and unhide sections of code for outlining,"
			<< "using mouse clicks."
			<< endl;
	}
	void demoReq6() {
		req(6);
		cout << "Embed in each web page's <head> section links to the style sheet and JavaScript file."
			<< endl;
	}
	void demoReq7() {
		req(7);
		cout << "embedd HTML5 links to dependent files with a label, at the top of the web page."
			<< " Publisher shall use functionality from your Project #2 to discover package "
			<< "dependencies within the published set of source files."
			<< endl;
	}
	void demoReq8() {
		req(8);
		cout << "develop command line processing to define the files to publish by "
			<< "specifying path and file patterns."
			<< endl;
	}
	void demoReq9() {
		req(9);
		cout << "demonstrate the CodePublisher functionality by publishing all the "
			<< "important packages in your Project #3."
			<< endl;
	}
	
	void demoReq10() {
		req(10);
		cout << "include an automated unit test suite that demonstrates you meet "
			<< "all the requirements of this project"
			<< endl;
	}
};