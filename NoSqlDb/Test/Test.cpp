/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////


#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
//#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../XMLPersistent/XMLPersistent.h"
#include "Test.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------


auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<Payload_temp>& db() { return db_; }
private:
	static DbCore<Payload_temp> db_;
};

DbCore<Payload_temp> DbProvider::db_;






///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	std::cout << "We are in testR3a";
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	
	

	// create some demo elements and insert into db

	DbElement<Payload_temp> demoElem;

	demoElem.name("Prasanna");
	demoElem.descrip("Student for CSE687");
	demoElem.dateTime(DateTime().now());


	if (demoElem.name() != "Prasanna")
		return false;
	if (demoElem.descrip() != "Student for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	//if (demoElem.payLoad().payload_string != "Ho took OOD")
	//	return false;

	showHeader();
	showElem(demoElem);

	db["Dodwad"] = demoElem;
	db["Dodwad"].payLoad().payload_string("Ho took OOD");
	db["Dodwad"].payLoad().payload_category_vector().push_back("Semester1");
	db["Dodwad"].payLoad().payload_category_vector().push_back("Semester2");
	dbp.db() = db;
	showDb(db);
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	DbElement<Payload_temp> demoElem;
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	db["Salman"] = demoElem;
	db["Salman"].payLoad().payload_string("You should try ...");
	db["Salman"].payLoad().payload_category_vector().push_back("Microsoft");
	db["Salman"].payLoad().payload_category_vector().push_back("Google");
	if (!db.contains("Salman"))
		return false;
	demoElem.name("Jianan");
	db["Sun"] = demoElem;
	db["Sun"].payLoad().payload_string("Dr. Fawcett said ...");
	db["Sun"].payLoad().payload_category_vector().push_back("Adobe");
	db["Sun"].payLoad().payload_category_vector().push_back("Amazon");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	db["Prashar"].payLoad().payload_string("You didn't demonstrate Requirement #4");
	db["Prashar"].payLoad().payload_category_vector().push_back("Yahoo");
	db["Prashar"].payLoad().payload_category_vector().push_back("Facebook");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	db["Arora"].payLoad().payload_string("You didn't demonstrate Requirement #5");
	db["Arora"].payLoad().payload_category_vector().push_back("NVIDIA");
	db["Arora"].payLoad().payload_category_vector().push_back("Wikipidea");
	DbCore<Payload_temp>::Keys keys = db.keys();
	showKeys(db);
	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	std::cout << "\n  showing all the database elements:";
	dbp.db() = db;
	showDb(db);
	std::cout << "\n  database keys are: ";
	showKeys(db);
	return true;
}

//requirement to read xml and add all DBelements to Existing DB.
bool testR3c()
{
	std::cout << "\n*******Test.cpp****************TestR3c**************Reading DB through XML and adding to DB created in Requirement 3a and 3b**********************\n";
	DbProvider dbp;
	std::string xmlpath = "../Database.xml";
	XMLPersistent<Payload_temp> xml_P(dbp.db());
	dbp.db() = xml_P.create_DB_through_XML(xmlpath);
	showDb(dbp.db());
	return true;

}

//requirement to demonstrate augmented XML requirement where xml ket key wont be added back to DB if its already exist in DB
bool testR3d()
{
	std::cout << "\n*******Test.cpp****************TestR3d**************(Augmented XML)Reading one more XML and skipping key which is already in DB**********************\n";
	
	DbProvider dbp;
	std::string xmlpath = "../DatabaseAugmented.xml";
	XMLPersistent<Payload_temp> xml_P(dbp.db());
	dbp.db() = xml_P.create_DB_through_XML(xmlpath);
	std::cout << "\n \n --------there is a metadata change in Key Kolupoti in augmented xml and new key Midha which is not reflected in DB";
	showDb(dbp.db());
	
	return true;
}

//Demonstarating requirement adding key with children into existing DB.
bool testR4b()
{
	std::cout << "\n*******Test.cpp****************TestR4b*************Additon of Key with Children like Llaita added and kumar is children**********************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	DbElement<Payload_temp> demoElem;
	Payload_temp pl_for_dbellement;
	std::vector<std::string> dbelement_children;

	demoElem.name("Lalita");
	demoElem.descrip("She lives in Siwan Too");
	pl_for_dbellement.payload_string("She is Parent to Vijay Kumar");
	pl_for_dbellement.payload_category_vector().push_back("Karnal");
	dbelement_children.push_back("Kumar");

	addKey(db, "Chaudhary", demoElem, pl_for_dbellement, dbelement_children);

	dbp.db() = db;
	showDb(dbp.db());
	return true;
}
//Demonstarting requirement of deletion of key which is never a children to anyother key
bool testR4c()
{
	std::cout << "\n*******Test.cpp****************TestR4c*************Deletion of Key who does not have Parent like Parag**********************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	deleteKey(db, "Taneja");
	dbp.db() = db;
	showDb(dbp.db());
	return true;
}

// Demonstarting requirement of deletion of key which is  a children to other key/keys
bool testR4d()
{
	std::cout << "\n*******Test.cpp****************TestR4d*********Deletion of Key who has Parent like Vijay**********************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	deleteKey(db, "Kumar");
	dbp.db() = db;
	showDb(dbp.db());
	return true;
}

//demonstartates requirement addintion of key to existing DB without children
bool testR4a()
{
	std::cout << "\n*******Test.cpp****************TestR4a**************Additon of Key without Children like bijay kumar added to DB**********************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	DbElement<Payload_temp> demoElem;
	Payload_temp pl_for_dbellement;

	demoElem.name("Vijay");
	demoElem.descrip("He lives in Siwan");
	pl_for_dbellement.payload_string("HE lives in India");
	pl_for_dbellement.payload_category_vector().push_back("Harayana");
	pl_for_dbellement.payload_category_vector().push_back("Punjab");

	addKey(db , "Kumar" , demoElem , pl_for_dbellement);

	dbp.db() = db;
	showDb(dbp.db());
	testR4b();
	testR4c();
	testR4d();
	return true;
}





//Demonstrating editing of metadata for a specific  key.
bool testR5a()
{
	std::cout << "\n*******Test.cpp****************TestR5a*********Editing of Metadata - Name , Description , Payload File Path*****changing Lalita Metadata*****************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	changeDbElementMetadata(db, "Chaudhary","Lalita-Changed","Lives in Siwan - Changed","She is Parent - Changed");
	dbp.db() = db;
	showDb(dbp.db());
	return true;
}

//Demonstarting movement of children from one parent to another parent.
bool testR5b()
{
	std::cout << "\n*******Test.cpp*************TestR5b*********Removing Sun from Ammar & adding him under Lalita-Changed *****************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	addRemoveRelation(db, "Salman", "Sun", "Remove");
	addRemoveRelation(db, "Chaudhary", "Sun","Add");
	dbp.db() = db;
	showDb(dbp.db());
	return true;
}

//Demonstarting requirement where metadta needs to be changed by changing instance of democelemnt instead of setter functions
bool testR5c()
{
	std::cout << "\n*******Test.cpp*************TestR5c*********Replacing instance of Nikhil with new instance*****************\n";
	DbProvider dbp;
	DbCore<Payload_temp> db = dbp.db();
	replacingInstance(db, db["Prashar"],"Prashar");
	dbp.db() = db;
	showDb(dbp.db());
	return true;
}

//Demonstarting query where key matches condition
bool testR6a()
{
	std::cout << "\n*******Test.cpp*************TestR6a*********Return key values ex key = fawcett*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_key(dbp.db());
	Condition<Payload_temp> cond_key;
	cond_key.key("Fawcett");

	query_key.select(cond_key).show();
	return true;
}

//Demonstarting query where chdren of key matches condition
bool testR6b()
{
	std::cout << "\n*******Test.cpp*************TestR6b*********Select children of key*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_children(dbp.db());
	Condition<Payload_temp> cond_children;
	cond_children.children("Salman");

	query_children.select(cond_children).show();
	return true;
}

//Demonstarting query where key matches condition of name
bool testR6d()
{
	std::cout << "\n*******Test.cpp*************TestR6d*********Metadata - Return keys value where Name macthes Pranjul  *****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_name(dbp.db());
	Condition<Payload_temp> cond_name;
	cond_name.name("Pranjul");

	query_name.select(cond_name).show();
	return true;
}

//Demonstarting query where key matches condition of description or its pattern
bool testR6e()
{
	std::cout << "\n*******Test.cpp*************TestR6e*********Metadata - Return keys value where description has pattern (TA(.*) *****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_description(dbp.db());
	Condition<Payload_temp> cond_description;
	cond_description.description("(TA)(.*)");

	query_description.select(cond_description).show();
	return true;
}

//Demonstarting query where key matches condition of payload or its pattern
bool testR6f()
{
	std::cout << "\n*******Test.cpp*************TestR6f*********Metadata - Return keys value where Payload string macthes (Dr.\\sFawcett)(.*) and vector has Amazon and Adobe*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_payload_string(dbp.db());
	Condition<Payload_temp> cond_payload_string;
	Payload_temp p; p.payload_string("(Dr.\\sFawcett)(.*)");
	p.payload_category_vector().push_back("Adobe");
	p.payload_category_vector().push_back("Amazon");
	//cond_payload_string.payload_string("(Dr.\\sFawcett)(.*)");
	cond_payload_string.payload_set(p);
	//Dr.Fawcett said ...
	query_payload_string.select(cond_payload_string).show();
	return true;
}
//Demonstarting query where key matches condition pattern
bool testR6c()
{
	std::cout << "\n*******Test.cpp*************TestR6c*********Return vale of all keys start with S *****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_key(dbp.db());
	Condition<Payload_temp> cond_key;
	cond_key.key("(S)(.*)");

	query_key.select(cond_key).show();

	testR6d();
	testR6e();
	testR6f();

	return true;
}



//Demonstarting query where key matches condition of date time interval
bool testR6g()
{
	std::cout << "\n*******Test.cpp*************TestR6g*********Keys between Date Interval*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_date_between(dbp.db());
	Condition<Payload_temp> cond_date_time, cond_date_time1;
	cond_date_time.date_time("Mon Jan 29 06:53:26 2018", "Mon Jan 30 06:53:31 2018");
	//cond_date_time1.children("Salman");
	//query_date_between.select(cond_date_time).select(cond_date_time1).show();
	query_date_between.select(cond_date_time).show();
	return true;
}

//Demonstarting query where key matches condition of date time interval with finish time empty
bool testR6h()
{
	std::cout << "\n*******Test.cpp*************TestR6h*********Keys between Date Interval where end date is null*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_date_between(dbp.db());
	Condition<Payload_temp> cond_date_time, cond_date_time1;
	cond_date_time.date_time("Mon Jan 29 06:53:26 2018", "");
	query_date_between.select(cond_date_time).show();
	return true;
}
//Demonstarting query union where key matches various conditions.
bool testR7a()
{
	std::cout << "\n*******Test.cpp*************TestR7a*********OR query where name = Pranjul OR key = Salman*****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_OR(dbp.db());
	Condition<Payload_temp> cond_name, cond_key;
	cond_name.name("Pranjul");
	cond_key.key("Salman");
	query_OR.select(cond_name).unionOfQuery().select(cond_key).show();

	return true;
}

//Demonstarting query AND where key matches various conditions
bool testR7b()
{
	std::cout << "\n*******Test.cpp*************TestR7b*********OR query where description = (TA)(.*) AND name = Jianan AND key = (S)(.*) *****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_OR(dbp.db());
	Condition<Payload_temp> cond_name, cond_description,cond_key;
	cond_name.name("Jianan");
	cond_key.key("(S).*");
	cond_description.description("(TA)(.*)");
	query_OR.select(cond_description).andOfQuery().select(cond_key).select(cond_name).show();

	return true;
}

//Demonstarting query AND , OR where key matches various conditions
bool testR7c()
{
	std::cout << "\n*******Test.cpp*************TestR7b*********OR , AND Mix query where description = (TA)(.*) AND key = (S)(.*) OR NAme= Parag *****************\n";
	DbProvider dbp;
	Query<Payload_temp> query_OR(dbp.db());
	Condition<Payload_temp> cond_name, cond_description, cond_key;
	cond_name.name("Parag");
	cond_key.key("(S).*");
	cond_description.description("(TA)(.*)");
	query_OR.select(cond_description).andOfQuery().select(cond_key).unionOfQuery().select(cond_name).show();

	return true;
}

//Demonstarting writing DB back to XML file
bool testR8a()
{
	std::cout << "\n*******Test.cpp****************TestR8a*********Writing DB to new XML file**********************\n";
	DbProvider dbp;
	XMLPersistent<Payload_temp> xml_P(dbp.db());
	xml_P.write_XML_through_DB(dbp.db(),"DataBase", "../../DB.xml");
	return true;
}




using namespace Utilities;

void Test::set(DbCore<Payload_temp> db)
{
	DbProvider dbp;
	dbp.db() = db;
	showDb(db);
}

DbCore<Payload_temp> Test::execute()
{
	DbProvider dbp;
	TestExecutive ex;
	TestExecutive::TestStr ts1{ testR1, "\n Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "\n Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "\n Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3b, "\n Create DbCore<std::string>" };
	TestExecutive::TestStr ts3c{ testR3c, "\n Reading XML and adding to DB" };
	TestExecutive::TestStr ts3d{ testR3d, "\n Creating one more XML and skipping keys which is already there in DB" };
	TestExecutive::TestStr ts4a{ testR4a, "\n Adding Key w/o children , with children , deleting key w/o dpendency , deleting key with dpeendency" };
	TestExecutive::TestStr ts5a{ testR5a, "\n Editing of Metadata - Name , Description , Payload File Path" };
	TestExecutive::TestStr ts5b{ testR5b, "\n Addition/Deletion of Relationship" };
	TestExecutive::TestStr ts5c{ testR5c, "\n Creating a new Instance of DBElement ,Replacing the existing with some edits" };
	TestExecutive::TestStr ts6a{ testR6a, "\n Vlaue of Specified Key" };
	TestExecutive::TestStr ts6b{ testR6b, "\n Children of specifed key" };
	TestExecutive::TestStr ts6c{ testR6c, "\n Values of key based on Regualar Expression" };
	TestExecutive::TestStr ts6g{ testR6g, "\n Keys between Date Interval" };
	TestExecutive::TestStr ts6h{ testR6h, "\n Keys between Date Interval with end date null" };
	TestExecutive::TestStr ts7a{ testR7a, "\n OR of two or more queries by setting two or more consitions" };
	TestExecutive::TestStr ts7b{ testR7b, "\n AND of two or more queries by setting two or more consitions" };
	TestExecutive::TestStr ts7c{ testR7c, "\n AND-OR of two or more queries by setting two or more conditions" };
	TestExecutive::TestStr ts8a{ testR8a, "\n Writing DB to new XML file" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts3c);
	ex.registerTest(ts3d);
	ex.registerTest(ts4a);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	ex.registerTest(ts5c);
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts6g);
	ex.registerTest(ts6h);
	ex.registerTest(ts7a);
	ex.registerTest(ts7b);
	ex.registerTest(ts7c);
	ex.registerTest(ts8a);
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	return dbp.db();
}

//----< test stub >----------------------------------------------------
#ifdef TEST_DBCORE
int main()
{
	DbProvider dbp;
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	std::getchar();
	return 0;
}
#endif
