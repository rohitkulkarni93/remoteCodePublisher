
//////////////////////////////////////////////////////////////////////
//  Project4_testExecutive.cpp - Test Suite to demostrate //
//   requirements //
//  version 1.1                                                     //
//  Language:     C++, Visual Studio 2015                           //
//  Application:  Dependency-Based CodePublisher        //
//  CSE687 - Object Oriented Design                   //
//  Author:       Rohit Kulkarni                                     //
//////////////////////////////////////////////////////////////////////


#include "TestExecutiveProject4.h"
#include <Windows.h>

using namespace std;

// ------- <Utility function for display > ---------
void TestExecutiveProject4::Print(std::string str)
{
	std::cout << std::endl << "=====================================================================================" << std::endl;
	std::cout << str << std::endl;
	std::cout << "=====================================================================================" << std::endl;
}


// ----------- < req 1 demo > -----------
void TestExecutiveProject4::Req1()
{
	std::cout << "This project is developed using Visual Studio 2015 and its C++ Windows Console Projects.\n It also uses Windows Presentation Founder(WPF) to provide Graphical User Interface(GUI)." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 2 demo > -----------
void TestExecutiveProject4::Req2()
{
	std::cout << "Uses the C++ standard library's streams for all console I/O and new and delete \n for all heap-based memory management." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 3 demo > -----------
void TestExecutiveProject4::Req3()
{
	std::cout << "Provide a Repository program that provides functionality to publish, as linked \n web pages, the contents of a set of C++ source code files.." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 4 demo > -----------
void TestExecutiveProject4::Req4()
{
	std::cout << " For the Publishing process, satisfies the requirements of CodePublisher developer \n in Project #3" << std::endl;
	std::cout << std::endl;
}

// ----------- < req 5 demo > -----------
void TestExecutiveProject4::Req5()
{
	std::cout << "Provides a Client Programme that can upload files, and view repository contents." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 6 demo > -----------
void TestExecutiveProject4::Req6()
{
	std::cout << "Provides a message-passing communication system, based on Sockets, used to access \n the Repository's functionality from another process or machine." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 7 demo > -----------
void TestExecutiveProject4::Req7()
{
	std::cout << "Provides the support for passing HTTP style messages using either synchronous request/response \n or asynchronous one-way messaging." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 8 demo > -----------
void TestExecutiveProject4::Req8()
{
	std::cout << "The communication channel supports the sending and recieving streams of bytes." << std::endl;
	std::cout << std::endl;
}

// ----------- < req 9 demo > -----------
void TestExecutiveProject4::Req9()
{
	std::cout << "Includes an automated unit test suite that demonstrates all the requirements \n of this project have been met." << std::endl;
	std::cout << std::endl;
}


void showUsage();

#ifdef test
int main(int argc, char* argv[])
{
	//Calling all Functions functions
	TestExecutiveProject4::Print("Demonstrating Req 1");
	TestExecutiveProject4::Req1();
	TestExecutiveProject4::Print("Demonstrating Req 2");
	TestExecutiveProject4::Req2();
	TestExecutiveProject4::Print("Demonstrating Req 3");
	TestExecutiveProject4::Req3();
	TestExecutiveProject4::Print("Demonstrating Req 4");
	TestExecutiveProject4::Req4();
	TestExecutiveProject4::Print("Demonstrating Req 5");
	TestExecutiveProject4::Req5();
	TestExecutiveProject4::Print("Demonstrating Req 6");
	TestExecutiveProject4::Req6();
	TestExecutiveProject4::Print("Demonstrating Req 7");
	TestExecutiveProject4::Req7();
	TestExecutiveProject4::Print("Demonstrating Req 8");
	TestExecutiveProject4::Req8();
	TestExecutiveProject4::Print("Demonstrating Req 9");
	TestExecutiveProject4::Req9();
	TestExecutiveProject4::Print("Demonstrating Req 10");

	return 0;
}
#endif