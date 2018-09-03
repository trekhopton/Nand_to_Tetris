//a1696375
#include "myTable.h"


//add a key, value pair to the table
void table::insert(std::string key, std::string value){

	keys[key] = value;
	return;
}

//check if the
bool table::contains(std::string key){

	if(keys.find(key) != keys.end()){
		return true;
	}
	return false;
}

std::string table::lookup(std::string key){

	return keys.find(key)->second;
}
