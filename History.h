#pragma once
#include <string>

class History
{
public:
	History();
	History(std::string datetime, std::string title);
	~History();

	std::string datetime;
	std::string title;
	std::string description;
};

