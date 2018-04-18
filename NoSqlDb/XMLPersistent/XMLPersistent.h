#pragma once




/*
/////////////////////////////////////////////////////////////////////////////////////
// XMLPersistent.h - represents XML Creation, XML Reading , writing XML to File     //
// ver 1.0                                                                         //
// Language:    C++, Visual Studio 2017                                           //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2017                  //
////////////////////////////////////////////////////////////////////////////////////


/*
Package Operations:
* -------------------
* This package provides 1 class:
*
- XMLPersistent : Template class to demonstrate XML operations
The member functions are :

1) write_XML_through_DB(const DbCore<T>& param_write_db, std::string root_element_string,std::string filename_with_filepath) : Creates/overides an  XML file , with Database content 'db' and
Filename 'filename'

2) create_DB_through_XML(std::string path_to_xml) : creates DB the though xml file and retursn same DB for its persistence.

3) write_xml_to_file(std::string xml,std::string  filename_with_filepath) :  Writes the xml string into file specified

4) read_DBElement_metadata(DbCore<T>& param_complete_db, std::shared_ptr<AbstractXmlElement> itr, std::string keyname): handler function for Key , Name , description and datetime

5) read_Payload(DbCore<T>& param_complete_db, std::shared_ptr<AbstractXmlElement> itr, std::string keyname): handles payload while reading XML


* Required Files:
* ---------------
* DbCore.h
* XmlParser.h,XMLDocument.h

* Maintenance History:
* --------------------
* ver 1.0 : 7th Feb 2018






*/
#include<iostream>
#include<string>
#include"../DbCore/DbCore.h"
#include "../../XmlDocument/XmlParser/XmlParser.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include <fstream>

using namespace NoSqlDb;
using namespace XmlProcessing;

template <typename T>
//XML PERsiistnet class with its member functions and variables.
class XMLPersistent
{
private:
	DbCore<T> complete_db;

public:
	//constructot to set DB in use.
	XMLPersistent(const DbCore<T>& db) {
		complete_db = db;
	}
	bool write_xml_to_file(std::string xml,std::string  filename_with_filepath);
	bool write_XML_through_DB(const DbCore<T>& param_write_db, std::string root_element_string,std::string filename_with_filepath);
	DbCore<T>& create_DB_through_XML(std::string path_to_xml);
	bool test();
	

};
template<typename T>
//This functions simply writes the string which is basically a DB into File by appending XML starting lines.
bool XMLPersistent<T>::write_xml_to_file(std::string xml, std::string filename_with_filepath)
{
	//std::fstream file; // use fstream (for reading and writing)
	std::string xml_head = "< ? xml version = \"1.0\" encoding = \"utf-8\" ? >\n";
	xml_head = xml_head + xml;

	std::string filename = filename_with_filepath; //"../../DB.xml";
	std::ofstream outfile(filename);
	outfile << xml_head << std::endl;
	outfile.close();
	return true;
}

template<typename T>
//DB is read and converted into cml struction through xmlparser and xmldocument and then converted to string.
bool XMLPersistent<T>::write_XML_through_DB(const DbCore<T>& param_write_db, std::string root_element_string, std::string filename_with_filepath)
{
	using sptr = std::shared_ptr<AbstractXmlElement>;
	//DbProvider1 dbp;
	sptr root = makeTaggedElement(root_element_string);
	for (auto item : param_write_db.dbStore())
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
	write_xml_to_file(docEl->toString(),filename_with_filepath);
	return true;
}

template<typename T>
//Read XML and every node of DBElement is added to database.
inline DbCore<T>& XMLPersistent<T>::create_DB_through_XML(std::string path_to_xml)
{
	XmlParser parser(path_to_xml);
	parser.verbose();
	XmlDocument* pDoc = parser.buildDocument();
	using sptr = std::shared_ptr<AbstractXmlElement>;

	//Select the root element and return all the childrens below root
	std::vector<sptr> vec = pDoc->elements("DataBase").select();
	std::string keyname;
	bool flag_key_exists, flag_do_not_enter_if;

	for (auto x : vec) {
		keyname = "";
		flag_key_exists = false;
		flag_do_not_enter_if = false;
		for (auto y : x->children()) {

			if (y->value() == "payload") {
				if (flag_key_exists == false)
				read_Payload(this->complete_db,y, keyname);
			}
			else if (y->value() == "children") {
				for (auto b : y->children()) {
					if (flag_key_exists == false)
					this->complete_db[keyname].children().push_back(b->children()[0]->value());
				}
			}
			else {
				std::cout << y->children()[0]->value();
				if(flag_key_exists == false)
				keyname = read_DBElement_metadata(this->complete_db, y, keyname);
				if (this->complete_db.contains(keyname) && flag_do_not_enter_if == false)
				{
					flag_key_exists = true;
				}
				flag_do_not_enter_if = true;
			}
		}
	}
	return this->complete_db;
}

template<typename T>
inline bool XMLPersistent<T>::test()
{
	return false;
}

template<typename T>
//hadles the 4 different nodes of xml which are key , name , description and date_time
std::string read_DBElement_metadata(DbCore<T>& param_complete_db, std::shared_ptr<AbstractXmlElement> itr, std::string keyname)
{

	if (itr->value() == "name" ) {
		param_complete_db[keyname].name() = itr->children()[0]->value();
	}
	if (itr->value() == "description") {
		param_complete_db[keyname].descrip() = itr->children()[0]->value();
	}
	if (itr->value() == "date_time") {
		param_complete_db[keyname].dateTime() = itr->children()[0]->value();
	}
	if (itr->value() == "key") {
		keyname = itr->children()[0]->value();
	}

	return keyname;
}

template<typename T>
//Handles the payload tag of xml as it as two more tag one for path and another for category.
std::string read_Payload(DbCore<T>& param_complete_db, std::shared_ptr<AbstractXmlElement> itr, std::string keyname)
{
	for (auto itr_category : itr->children()) {
		if (itr_category->value() == "payload_category") {
			for (auto itr_category_type : itr_category->children()) {
				param_complete_db[keyname].payLoad().payload_category_vector().push_back(itr_category_type->children()[0]->value());
			}
		}
		else {
			param_complete_db[keyname].payLoad().payload_string(itr_category->children()[0]->value());
		}
	}
	return keyname;
}