#include "XMLPersistent.h"
#include "../DbCore/DbCore.h"
#include <string>
#include "../../XmlDocument/XmlParser/XmlParser.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include <fstream>
#include<iostream>

using namespace NoSqlDb;
using namespace XmlProcessing;

#ifdef TEST_XMLPERSISTENT

int main() {

	DbCore<Payload_temp> db;
	DbElement<Payload_temp> demoelem;
	demoelem.name("Sachin");
	demoelem.descrip("He is the best cricketer so far");
	demoelem.dateTime(DateTime().now());
	demoelem.payLoad().payload_string("He plays for India");
	demoelem.payLoad().payload_category_vector().push_back("Test Cricket");
	demoelem.payLoad().payload_category_vector().push_back("One Day cricket");
	db["Tendulkar"] = demoelem;
	Persistance<Payload_temp> p;

	XMLPersistent<Payload_temp> xml_P(db);
	xml_P.write_XML_through_DB(db, "DataBase","PersistentXMLTestStub.xml");
	
	std::getchar();
	return 0;
}

#endif
