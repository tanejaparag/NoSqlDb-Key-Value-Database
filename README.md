# NoSqlDb-Key-Value-Database
How a non SQL database can be constructed and used.

NoSqlDb Key/Value Database project:
- implement in C++ using the facilities of the standard C++ Libraries and Visual Studio 2017, as provided in the ECS clusters.
- use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management1.
- implements a generic key/value in-memory database where each value consists of:
    Metadata:
      A name string
      A text description of the item
      A DateTime instance recording the date and time the value was written to the database.
      a finite number (possibly zero) of child relationships with other values. Each element of the child relationships collection is the key of another item in the database. Any item holding a key in this collection is the parent of the element accessed by the key.
    An instance of the generic type1. This might be a string, a container of a set of values of the same type, or some other collection of data captured in some, perhaps custom, data structure. We will refer to this instance as the database element's payload.
- support addition and deletion of key/value pairs.
- upport editing of values including the addition and/or deletion of relationships, editing text metadata, and replacing an existing value's instance with a new instance. Editing of keys is forbidden.
If you delete child relationships, e.g., delete keys from an element's child collection, you are forbidden to delete the elements accessed from those keys from the database.
- support queries2 for:
    The value of a specified key.
    The children of a specified key.
    The set of all keys matching a specified regular-expression pattern.
    All keys that contain a specified string in their metadata section, where the specification is based on a regular-expression pattern.
    All keys that contain values written within a specified time-date interval. If only one end of the interval is provided shall take the present as the other end of the interval.
- support queries on the set of keys returned by a previous query3, e.g., an "and"ing of multiple queries. Shall also support queries on the union of results of one or more previous queries, e.g., an "or"ing of multiple queries.
- on command, persist a colllection of database contents, defined by a collection of keys, to an XML file4,5. It is intended that this process be reversible, e.g., the database can be restored or augmented6 from an existing XML file as well as write its contents out to one or more XML files.
- implement a Payload type that contains a string, which for Project #2, will be the path to a file in the Repository. The Payload type shall also contain a std::vector<std::string>, where each string is the name of a category associated with the Payload file. For the Repository, the key will be a file name, so the combination of key and payload will provide details about a file stored in the Repository.
- provide, in your implementation, at least the following packages: Executive, DBCore, Query, Test.
- submitted with contents, in the form of an XML file, that describe your project's package structure and dependency relationships that can be loaded when your project is graded.
- accompanied by a test executive that clearly demonstrates you've met all the functional requirements #2-#9, above7. If you do not demonstrate a requirement you will not get credit for it even if you have it correctly implemented.
- provide a pdf file containing a package diagram8 of your project.
