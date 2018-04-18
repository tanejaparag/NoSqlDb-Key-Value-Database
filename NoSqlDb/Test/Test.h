////////////////////////////////////////////////////////////////////////////
// Test.h - Provides the Package to run test cases                       //
// ver 1.0                                                                //
// Language:    C++, Visual Studio 2017                                  //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////

/*
Package Operations:
* -------------------
* This package provides operations to test the test cases
* -

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* Query.h, Query.cpp
* Persistance.h, Persistance.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 7 Feb 2018


*/



#include "../DbCore/DbCore.h"
#include<iostream>

using namespace NoSqlDb;
//class Test which has function to set DB in use and execute all the test cases which is called in Execute.
class Test
{

public:
	
	void set(DbCore<Payload_temp> db);
	DbCore<Payload_temp> execute(void);
	
};