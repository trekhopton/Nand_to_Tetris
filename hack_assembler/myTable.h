//a1696375
#ifndef MYTABLE_H
#define MYTABLE_H

#include <map>

class table{
public:
	//add a key, value pair to the table
	void insert(std::string key, std::string value);
	//check if the key exists in the table
	bool contains(std::string key);
	//look up the variable
	std::string lookup(std::string key);
private:
	std::map<std::string, std::string> keys;
};

#endif
