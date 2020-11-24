#define CATCH_CONFIG_MAIN
#define DEBUG
#include "catch.hpp"
#include "Washington_FaultSeeded.cpp"
#include <iostream>
#include <string>
using namespace std;

TEST_CASE ("Test cases for classRecords"){
	ClassRecords testrecords;
	string name = string("tony hawk");
	string uid = string("40");
	string email = string("tonyhawk@example.net");
	testrecords.addRecord(name,uid,email);

	string name2 = string("bob ross");
	string uid2 = string("12345678");
	string email2 = string("bobross@example.net");
	testrecords.addRecord(name2,uid2,email2);
	testrecords.addRecord(name2,uid2,email2);

	//zero should be the index of tony
	REQUIRE(testrecords.getSize() == 3);
	REQUIRE(testrecords.searchRecords(name) == 0 );
	REQUIRE(testrecords.searchRecords(name2) == 1 );

}
