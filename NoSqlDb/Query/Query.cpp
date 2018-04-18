
/////////////////////////////////////////////////////////////////////
// Query.cpp -   Test stub for Query Project                       //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2017                            //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2018   //
/////////////////////////////////////////////////////////////////////




#include "Query.h"
#include <iostream>

using namespace NoSqlDb;



#ifdef TEST_QUERY



int main() {

	Condition<Payload_temp> c;
	c.name("Sachin");


	DbCore<Payload_temp> db;
	DbElement<Payload_temp> demoelem;
	demoelem.name("Sachin");
	demoelem.descrip("He is the best cricketer so far");
	demoelem.dateTime(DateTime().now());
	demoelem.payLoad().payload_string("He plays for India");
	demoelem.payLoad().payload_category_vector().push_back("Test Cricket");
	demoelem.payLoad().payload_category_vector().push_back("One Day cricket");
	db["Tendulkar"] = demoelem;

	Query<Payload_temp> q(db);

	q.select(c).show();

	std::getchar();
	return 0;
}

#endif 


