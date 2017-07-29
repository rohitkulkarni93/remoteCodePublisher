#pragma once
//////////////////////////////////////////////////////////////////////
//  TestExecutiveProject4.h - Test Suite to demostrate all        //
//   requirements				//
//  ver 1.1                                                         //
//  Language:     C++, Visual Studio 2015                           //
//  Application: Dependency-Based CodePublisher        //
//  CSE687 - Object Oriented Design                   //
//  Author:      Rohit Kulkarni              //
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package contains a test suite that demonstrates all the requirements
of project 4.

Public Interface functions:
Title(std::string str);
Req1()
Req2()
Req3()
Req4()
Req5()
Req6()
Req7()
Req8()
Req9()
Req10()
*/

#include<string>
#include <iostream>
#include<string>
#include<vector>

using namespace std;


/////////////////////////////////////////////////////////////////////
// Test executive class
/////////////////////////////////////////////////////////////////////
// - Includes an automated unit test suite that demonstrates all the requirements \n of this project have been met. 
class TestExecutiveProject4
{
public:
	static void Print(std::string str);
	static void Req1();
	static void Req2();
	static void Req3();
	static void Req4();
	static void Req5();
	static void Req6();
	static void Req7();
	static void Req8();
	static void Req9();
};