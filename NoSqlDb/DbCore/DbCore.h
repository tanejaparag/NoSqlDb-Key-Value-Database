#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include "../DateTime/DateTime.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <regex>







namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

	
	
	
	/*template<typename T>
struct Payload_temp {
		T payload_string;
		std::vector<T> payload_category_vector;
	};*/

struct Payload_temp {
	public:
		std::string& payload_string() { return payload_string_; }
		std::string payload_string() const { return payload_string_; }
		void payload_string(const std::string& name) { payload_string_ = name; }


		std::vector<std::string>& payload_category_vector() { return  payload_category_vector_; }
		std::vector<std::string> payload_category_vector() const { return  payload_category_vector_; }
		void payload_category_vector(const std::vector<std::string>& children) { payload_category_vector_ = children; }

		std::string payload_data() const { return payload_string_; }
		std::string& payload_data() { return payload_string_; }
		void payload_data(const std::string& name) { payload_string_ = name; }
		friend std::ostream& operator<<(std::ostream& os, const Payload_temp p);
		friend bool operator==(const Payload_temp& p_lhs , const Payload_temp& p_rhs);
		friend bool operator!=(const Payload_temp& p_lhs, const Payload_temp& p_rhs);

	private:
		std::string payload_string_;
		std::vector<std::string> payload_category_vector_;
		std::string payload_data_;
	};

template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

	T& payLoad() { return payLoad_; }
	T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
	T payLoad_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;


	//std::ostream& operator<<(Payload_temp p);
	//std::ostream& operator<<(std::ostream& out, Payload_temp p);

    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
	
	//std::ostream& operator<<( Payload_temp p);
  private:
    DbStore dbStore_;
    bool doThrow_ = false;

  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<T>());
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }

    /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload-Filepath";
	out << std::setw(25) << std::left << "Payload-FileCategory";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-----------------------";
  }

  inline std::ostream & operator<<(std::ostream & out, const Payload_temp p)
  {

	  out << std::setw(25) << std::left << p.payload_string();
	  if (p.payload_category_vector().size() > 0)
	  {
		  std::string temp_string;
		  for (auto key : p.payload_category_vector())
		  {
			  temp_string.append(key);
			  temp_string.append(",");
		  }
		  out << std::setw(25) << std::left << temp_string;
	  }
	  else
	  {
		  out << std::setw(25) << std::left << "There is no Payload_Category ";
	  }
	  return out;
  }

  inline bool operator==(const Payload_temp & p_lhs, const Payload_temp & p_rhs)
  {
	  std::regex temp_regex(p_lhs.payload_string());
	  if (std::regex_match(p_rhs.payload_string(), temp_regex))
	  {
		  if (p_lhs.payload_category_vector() == p_rhs.payload_category_vector())
		  {
			  return true;
		  }
		  else
		  {
			  return false;
		  }
	  }
	  else
		  return false;
  }
  inline bool operator!=(const Payload_temp & p_lhs, const Payload_temp & p_rhs)
  {
	  if (p_lhs.payload_string() != "" || p_lhs.payload_category_vector().size() > 0)
		  return true;
	  else
		  return false;
  }
  //----< display DbElements >-----------------------------------------
  /*template<typename T>
  std::ostream& DbCore<T>::operator<<(std::ostream& out, Payload_temp p)
  {
	  out << std::setw(25) << std::left << p.payload_string();
	  if (p.payload_category_vector().size() > 0)
	  {
		  std::string temp_string;
		  for (auto key : p.payload_category_vector())
		  {
			  temp_string.append(key);
			  temp_string.append(",");
		  }
		  out << std::setw(25) << std::left << temp_string;
	  }
	 /* else
	  {
		  out << std::setw(25) << std::left << "There is no Payload_Category ";
	  }
	  return out;
	  
  }*/


  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    //out << std::setw(25) << std::left << el.payLoad();
	std::cout << el.payLoad();
	/*Payload_temp Payload_instance = el.payLoad();
	out << std::setw(25) << std::left << Payload_instance.payload_string();

	if (Payload_instance.payload_category_vector().size() > 0)
	{
		std::string temp_string;
		for (auto key : Payload_instance.payload_category_vector())
		{
			temp_string.append(key);
			temp_string.append(",");
		}
		out << std::setw(25) << std::left << temp_string;
	}
	else
	{
		out << std::setw(25) << std::left << "There is no Payload_Category ";
	}
	*/
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }
  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }

  template<typename T>
  void addKey(DbCore<T>& param_db, const std::string& param_key, const DbElement<T>& param_el, const Payload_temp &param_payload)
  {
	  param_db[param_key] = param_el;
	  param_db[param_key].payLoad().payload_string(param_payload.payload_string());
	  
	  
	  std::vector<std::string> temp_vec = param_payload.payload_category_vector();

	  for (auto a : temp_vec)
	  {
		  param_db[param_key].payLoad().payload_category_vector().push_back(a);
	  }
	 
  }
  template<typename T>
  void addKey( DbCore<T>& param_db, const std::string& param_key, const DbElement<T>& param_el, const Payload_temp &param_payload,std::vector<std::string> param_children )
  {
	  addKey(param_db, param_key, param_el, param_payload);

	  for (auto a : param_children)
	  {
		  param_db[param_key].children().push_back(a);
	  }

  }

  template<typename T>
  void deleteKey(DbCore<T>& param_db, const std::string& param_keyload)
  {
	  if (param_db.contains(param_keyload)) {
		  for (auto item : param_db) {
			  std::string param_db_dbelement_key = item.first;
			  DbElement<std::string>::Children param_db_dbelement_key_children = item.second.children();
			  if (param_db_dbelement_key_children.size() > 0) {
				  int temp_count = 0;
				  for (std::vector<std::string>::iterator itr = param_db_dbelement_key_children.begin(); itr != param_db_dbelement_key_children.end(); itr++) {
					  if (*itr == param_keyload) {
						  param_db[param_db_dbelement_key].children().erase((param_db[param_db_dbelement_key].children().begin()) + temp_count);
						  temp_count++;
						  break;
					  }
					  else {
						  temp_count++;
					  }
				  }
			  }
		  }
		  param_db.dbStore().erase(param_keyload);
	  }
  }

  template<typename T>
  void changeDbElementMetadata(DbCore<T>& param_db, const std::string& param_key, const std::string& param_Name, const std::string& param_description, const std::string& param_payload_filepath)
  {
	  if (param_db.contains(param_key)) {

		  if (param_Name != "")
		  {
			  param_db[param_key].name() = param_Name;
		  }

		  if (param_description != "")
		  {
			  param_db[param_key].descrip() = param_description;
		  }

		  if(param_payload_filepath != "")
		  { 
			  param_db[param_key].payLoad().payload_string(param_payload_filepath);
		  }
	  }
	  else
	  {
		  std::cout << "Metadata data wont happen as Key does not exist in DB";
	  }

  }

  template<typename T>
  void addRemoveRelation(DbCore<T>& param_db, const std::string& param_key_parent, const std::string& param_key_child, const std::string& param_action)
  {
	  if (param_db.contains(param_key_parent) && param_db.contains(param_key_child))
	  {
		  if (param_action == "Add")
		  {
			  param_db[param_key_parent].children().push_back(param_key_child);
		  }
		  else if (param_action == "Remove")
		  {
			  if (param_db[param_key_parent].children().size() > 0)
			  {
				  int temp_count = 0;
				  int found = 0;
				  for (std::vector<std::string>::iterator itr = param_db[param_key_parent].children().begin(); itr != param_db[param_key_parent].children().end(); itr++) {
					  if (*itr == param_key_child) {
						  param_db[param_key_parent].children().erase((param_db[param_key_parent].children().begin()) + temp_count);
						  temp_count++;
						  found = 1;
						  break;
					  }
					  else {
						  temp_count++;
					  }
				  }
				  if (found == 0)
					  std::cout << "MEntiond child key was never a children to mentioned parent key";
			  }
			  else
				  std::cout << "Parent does not e=ven have children , Hence can not be removed";
		  }
		  else
			  std::cout << "Acttion Requested is wrong";
	  }
	  else
	  {
		  std::cout << "Addition/Delation won't haapen either parent key or child key does not exist in the DB";
	  }

  }

  template<typename T>
  void replacingInstance(DbCore<T>& param_db,DbElement<T> param_el,const std::string& param_key)
  {
	  if (param_db.contains(param_key))
	  {
		  param_el.name() = param_el.name() + "new instance";
		  param_el.descrip() = param_el.descrip() + "new instance";
		  param_db[param_key] = param_el;
	  }
	  else
		  std::cout << "MEntioned key is wrong";
  }
  
  
}
