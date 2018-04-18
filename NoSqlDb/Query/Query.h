#pragma once

////////////////////////////////////////////////////////////////////////////
// Query.h - Hasdesign of Query and Condition classes which helps in querying database with conditions  //
// ver 1.0                                                                //
// Language:    C++, Visual Studio 2017                                  //
// Parag Taneja, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////


/*
Package Operations:
* -------------------
* This package provides 2 classes:
*
- Query : Template Class ,  used to perform queries on database

Public Functions :

unionOfQuery() :  Used to set the flag true for OR query
andOfQuery() : Used to set the flag false for AND query
select() : This functios actual implements the searching with input as a Condition , where condition can of anytype like metadata , key , name , description
show() : This function shows the DB returned after applying conditions on main DB.

void PrintChildren(const std::string & key) :  Query to Select children of a particular key
void ValueofAKey(const std::string & key)  : Query to select the value of a particular key
void Datetime(Condition<T> c)  :               Query to select keys based on Datatime
void PrintRegexpkey(std::string exp)  :     Query to select key based on a regular expression

Below four fuctions are used to handle different scenarios of condition.
void handle_key(const DbCore<T>& a,Condition<T> c) : - This function executes when condition has key set in it.
void handle_name(const DbCore<T>& a, Condition<T> c) - This function executes when condition has name in it.
void handle_description(const DbCore<T>& a, Condition<T> c) - This function executes whene condition has description in it.
void handle_payload(const DbCore<T>& a, Condition<T> c) - This function exeuctes when condition has payload in it.


Query& select(Condition<T> c)  :   Query for cascading AND , OR queries
void show() :                          Shows the Database Keys with its value

- Condition : Template Class used to set the query condition

Its public functions are just the getter and setter functions for each attribute like name , key , payload , date time , description , children





* Required Files:
* ---------------
* DbCore.h, DateTime.cpp

*
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th Feb 2018

*/




#include "../DbCore/DbCore.h"
#include "../DateTime/DateTime.h"
#include <vector>
#include <string>
#include <regex>

using namespace NoSqlDb;

template <typename T>
// Template Condition class as same template can be used for DB insertion and selection.
class Condition
{
private: //all the private members are declared which are basically condition classes.
	std::string key_, description_, name_, date_time_start_, date_time_finish_, children_;// , payload_string_, ;
	T payload_;

public:
	//setter and getter function for key
	Condition & key(const std::string& s)
	{
		key_ = s;
		return *this;
	}
	std::string key() { return key_; }

	//setter and getter function for description
	Condition & description(const std::string& s)
	{
		description_ = s;
		return *this;
	}
	std::string description() { return description_; }

	//setter and getter function for name
	Condition & name(const std::string& s)
	{
		name_ = s;
		return *this;
	}
	std::string name() { return name_; }

	//setter and getter function for payload
	Condition<T> & payload_set(const T& s)
	{
		payload_ = s;
		return *this;
	}
	T& payload_get(){ return payload_; }
	//setter and getter function for Datetime
	std::string date_time_start() { return date_time_start_; }
	std::string date_time_finish() { return date_time_finish_; }
	void date_time(const std::string& s, const std::string& s1) { date_time_start_ = s; date_time_finish_ = s1;}
	//setter and getter function for children
	std::string children() { return children_; }
	void children(const std::string& s) { children_ = s; }
};

template <typename T>
//Template query class which handles the conditions on the DB
class Query
{
private:
	DbCore<T> complete_db;
	DbCore<T> temp_db;
	bool bool_and;
	void handle_key(const DbCore<T>& a,Condition<T> c);
	void handle_name(const DbCore<T>& a, Condition<T> c);
	void handle_description(const DbCore<T>& a, Condition<T> c);
	void handle_payload(const DbCore<T>& a, Condition<T> c);

public:
	//Constructor for Query class which sets the DB to be worked on
	Query(const DbCore<T>& db) {
		complete_db = db;

		bool_and = false;
	}

	//Decide the Anding and Oring of two queries basically two select
	Query& unionOfQuery() {

		bool_and = false;
		return *this;
	}
	//Decide the Anding and Oring of two queries basically two select
	Query& andOfQuery() {
		bool_and = true;
		return *this;
	}

	Query& select(Condition<T> c);
	DbCore<T> temp_DB() { return temp_db; }
	void show();

};



template<typename T>
//Returns all the keys which has key set in condition class
inline void Query<T>::handle_key(const DbCore<T>& param_action_db, Condition<T> param_c)
{
	DbCore<T> param_temp = param_action_db;
	std::regex temp_regex(param_c.key());
	for (auto x : param_temp)
	{
		if (std::regex_match(x.first, temp_regex))
		{
			if (!this->temp_db.contains(x.first))
			{
				this->temp_db[x.first] = param_temp[x.first];
				//dbp.db()[x.first] = complete_db[x.first];
			}
		}
		else if (bool_and == true && this->temp_db.contains(x.first))
		{
			this->temp_db.dbStore().erase(x.first);

		}
	}
}

template<typename T>
//Returns all the keys which has name set in condition class
inline void Query<T>::handle_name(const DbCore<T>& param_action_db, Condition<T> param_c)
{
	DbCore<T> param_temp = param_action_db;
	std::regex temp_regex(param_c.name());
	for (auto x : param_temp)
	{
		if (std::regex_match(x.second.name(), temp_regex))
		{
			if (!this->temp_db.contains(x.first))
			{
				this->temp_db[x.first] = param_temp[x.first];
				//dbp.db()[x.first] = complete_db[x.first];
			}
		}
		else if (bool_and == true && this->temp_db.contains(x.first))
		{
			this->temp_db.dbStore().erase(x.first);

		}
	}
}

template<typename T>
//Returns all the keys which has description set in condition class
inline void Query<T>::handle_description(const DbCore<T>& param_action_db, Condition<T> param_c)
{
	DbCore<T> param_temp = param_action_db;
	std::regex temp_regex(param_c.description());
	for (auto x : param_temp)
	{
		if (std::regex_match(x.second.descrip(), temp_regex))
		{
			if (!this->temp_db.contains(x.first))
			{
				this->temp_db[x.first] = param_temp[x.first];
				//dbp.db()[x.first] = complete_db[x.first];
			}
		}
		else if (bool_and == true && this->temp_db.contains(x.first))
		{
			this->temp_db.dbStore().erase(x.first);

		}
	}
}

/*
template<typename T>
inline void Query<T>::handle_payload(const DbCore<T>& param_action_db, Condition c)
{
	DbCore<T> param_temp = param_action_db;
	std::regex temp_regex(c.payload_string());

	for (auto x : param_temp)
	{
		if (std::regex_match(x.second.payLoad().payload_string(), temp_regex))
		{
			if (!this->temp_db.contains(x.first))
			{
				this->temp_db[x.first] = param_temp[x.first];
				//dbp.db()[x.first] = complete_db[x.first];
			}
		}
		else if (bool_and == true && this->temp_db.contains(x.first))
		{
			this->temp_db.dbStore().erase(x.first);

		}
	}
}
*/
template<typename T>
//Returns all the keys which has payload set in condition class
inline void Query<T>::handle_payload(const DbCore<T>& param_action_db, Condition<T> c)
{
	DbCore<T> param_temp = param_action_db;
	for (auto x : param_temp)
	{
		if (c.payload_get() == x.second.payLoad())
		{
			if (!this->temp_db.contains(x.first))
			{
				this->temp_db[x.first] = param_temp[x.first];
			}
		}
		else if (bool_and == true && this->temp_db.contains(x.first))
		{
			this->temp_db.dbStore().erase(x.first);
		}
	}
}

template<typename T>
//Returns all the keys depepnding on all the conditions set in condition class instance
inline  Query<T> & Query<T>::select(Condition<T> c)
{
	DbCore<T> action_db;
	if (bool_and == false)
		action_db = complete_db;
	else
		action_db = temp_DB();

	if (c.children() != "")
	{
		this->temp_db[c.children()] = action_db[c.children()];
	}
	if (c.date_time_start() != "")
	{
		DateTime start = DateTime(c.date_time_start());
		DateTime finish = DateTime().now();
		if (c.date_time_finish() != "")
			finish = DateTime(c.date_time_finish());
		for (auto x : action_db)
		{
			if (start < DateTime(x.second.dateTime()) && DateTime(x.second.dateTime()) < finish)
			{
				if (!this->temp_db.contains(x.first))
				{
					this->temp_db[x.first] = action_db[x.first];
				}
			}
			else if (bool_and == true && this->temp_db.contains(x.first))
			{
				this->temp_db.dbStore().erase(x.first);

			}
		}
	}

	if (c.key() != "")
		handle_key(action_db,c);

	if (c.name() != "")
		handle_name(action_db, c);

	if (c.description() != "")
		handle_description(action_db,c);

	Payload_temp k;
	if (c.payload_get() != k)
		handle_payload(action_db, c);
	return *this;
}

template<typename T>
//Show the DB keys with its values
inline void Query<T>::show()
{
	showDb(temp_DB());
}
