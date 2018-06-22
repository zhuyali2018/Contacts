#pragma once
#include <string>
#include <vector>
#include "History.h"
class Contact
{
public:
	Contact();
	Contact(int id, std::string name, std::string phone,std::string email);
	~Contact();

	int id;
	std::string name;
	std::string phone;		
	std::string email;
	std::string type;
	std::string country;
	std::string work;

	std::string company;
	std::string address;
	std::vector<History> histories;
};

class NoDataException : public std::exception {
public:
	const char * what() const throw() {
		return "Contact List: No Contact Exception";
	}
};