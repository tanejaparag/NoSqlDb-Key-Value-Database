



////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Provides the entry point for execution                 //
// ver 1.0                                                                //
// Language:    C++, Visual Studio 2017                                  //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////

/*

Class DbProvider : helps to share the Database between functions

Required Files:
* ---------------
* Test.h, Test.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 7 Feb 2018
*/

#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../../XmlDocument/XmlParser/XmlParser.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XMLPersistent/XMLPersistent.h"
#include "../DbCore/DbCore.h"
#include "../Test/Test.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <vector>
#include "Execute.h"
#include <fstream>


//using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

using namespace NoSqlDb;
using namespace XmlProcessing;

class DbProvider1
{
public:
	DbCore<Payload_temp>& db() { return db_; }
private:
	static DbCore<Payload_temp> db_;
};

DbCore<Payload_temp> DbProvider1::db_;
//bool create_DB_through_XML(std::string path_to_xml);
//std::string read_DBElement_metadata(std::shared_ptr<AbstractXmlElement> itr, std::string keyname);
//std::string read_Payload(std::shared_ptr<AbstractXmlElement> itr, std::string keyname);
//bool write_XML_through_DB(std::string root_element_string);
//bool write_xml_to_file(std::string xml);

int main()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<Payload_temp> db;
	DbProvider1 dbp;
	dbp.db() = db;


	
	//creating DB thorugh XML
	//bool bool_suuccessful_DB = create_DB_through_XML(xmlpath);


	db = dbp.db();
	
	// create some demo elements and insert into db

	DbElement<Payload_temp> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());

	


	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	//if (demoElem.payLoad().payload_string != "The good news is ...")
		//return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	db["Fawcett"].payLoad().payload_string("The good news is ...");
	db["Fawcett"].payLoad().payload_category_vector().push_back("Sem1");
	db["Fawcett"].payLoad().payload_category_vector().push_back("Sem2");
	

	dbp.db() = db;

	


	Test *tst1 = new Test();
	tst1->set(dbp.db());
	dbp.db() = tst1->execute();

	db = dbp.db();
	showDb(db);

	//write_XML_through_DB("DataBase");
	std::cout << "\n\n----------------Verify DB in DB.xml which is inside your unzip folder-------------\n\n";
	 
}

/*
bool create_DB_through_XML( std::string path_to_xml)
{
	DbProvider1 dbp;
	XmlParser parser(path_to_xml);
	parser.verbose();
	XmlDocument* pDoc = parser.buildDocument();
	using sptr = std::shared_ptr<AbstractXmlElement>;
	
	//Select the root element and return all the childrens below root
	std::vector<sptr> vec = pDoc->elements("DataBase").select();
	std::string keyname;

	for (auto x : vec) {
		keyname = "";
		for (auto y : x->children()) {

			if (y->value() == "payload") {
				read_Payload(y, keyname);
			}
			else if (y->value() == "children") {
				for (auto b : y->children()) {
					dbp.db()[keyname].children().push_back(b->children()[0]->value());
				}
			}
			else {
				std::cout << y->children()[0]->value();
				keyname = read_DBElement_metadata(y, keyname);
			}
		}
	}

	return true;
}

std::string read_DBElement_metadata(std::shared_ptr<AbstractXmlElement> itr,std::string keyname)
{
	DbProvider1 dbp;

	if (itr->value() == "name") {
		dbp.db()[keyname].name() = itr->children()[0]->value();
	}
	if (itr->value() == "description") {
		dbp.db()[keyname].descrip() = itr->children()[0]->value();
	}
	if (itr->value() == "date_time") {
		dbp.db()[keyname].dateTime() = itr->children()[0]->value();
	}
	if (itr->value() == "key") {
		keyname = itr->children()[0]->value();
	}

	return keyname;
}

std::string read_Payload(std::shared_ptr<AbstractXmlElement> itr, std::string keyname)
{
	DbProvider1 dbp;
	for (auto itr_category : itr->children()) {
		if (itr_category->value() == "payload_category") {
			for (auto itr_category_type : itr_category->children()) {
				dbp.db()[keyname].payLoad().payload_category_vector().push_back(itr_category_type->children()[0]->value());
			}
		}
		else {
			dbp.db()[keyname].payLoad().payload_string(itr_category->children()[0]->value());
		}
	}
	return keyname;
}
*/
/*
bool write_XML_through_DB(std::string root_element_string)
{
	using sptr = std::shared_ptr<AbstractXmlElement>;
	DbProvider1 dbp;
	sptr root = makeTaggedElement(root_element_string);
	for (auto item : dbp.db())
	{
		sptr key, dbElement, dbElement_name, dbElement_description, dbElement_date_time, dbElement_payload, dbElement_children, dbElement_payload_filepath, dbElement_payload_category;
		dbElement = makeTaggedElement("DBElement");
		key = makeTaggedElement("key");
		dbElement_name = makeTaggedElement("name");
		dbElement_description = makeTaggedElement("description");
		dbElement_date_time = makeTaggedElement("date_time");
		dbElement_payload = makeTaggedElement("payload");
		dbElement_children = makeTaggedElement("children");
		key->addChild(makeTextElement(item.first));
		dbElement_name->addChild(makeTextElement(item.second.name()));
		dbElement_description->addChild(makeTextElement(item.second.descrip()));
		dbElement_date_time->addChild(makeTextElement(item.second.dateTime()));
		dbElement_payload_filepath = makeTaggedElement("payload_filepath");
		dbElement_payload_filepath->addChild(makeTextElement(item.second.payLoad().payload_string()));
		dbElement_payload_category = makeTaggedElement("payload_category");
		dbElement_payload->addChild(dbElement_payload_filepath);
		dbElement_payload->addChild(dbElement_payload_category);
		for (auto itr_pl_cat : item.second.payLoad().payload_category_vector())
		{
			sptr dbElement_payload_category_type;
			dbElement_payload_category_type = makeTaggedElement("payload_category_type");
			dbElement_payload_category_type->addChild(makeTextElement(itr_pl_cat));
			dbElement_payload_category->addChild(dbElement_payload_category_type);
		}
		for (auto itr_child : item.second.children())
		{
			sptr dbElement_children_key;
			dbElement_children_key = makeTaggedElement("children_key");
			dbElement_children_key->addChild(makeTextElement(itr_child));
			dbElement_children->addChild(dbElement_children_key);
		}
		dbElement->addChild(key);
		dbElement->addChild(dbElement_name);
		dbElement->addChild(dbElement_description);
		dbElement->addChild(dbElement_date_time);
		dbElement->addChild(dbElement_payload);
		dbElement->addChild(dbElement_children);
		root->addChild(dbElement);
	}
	sptr docEl = makeDocElement(root);
	write_xml_to_file(docEl->toString());
	return true;
}
*/
/*
bool write_xml_to_file(std::string xml)
{
	//std::fstream file; // use fstream (for reading and writing)
	std::string xml_head = "< ? xml version = \"1.0\" encoding = \"utf-8\" ? >\n";
	xml_head = xml_head + xml;
	std::string filename = "../../DB.xml";
	std::ofstream outfile(filename);
	outfile << xml_head << std::endl;
	outfile.close();
	return true;

}
*/
