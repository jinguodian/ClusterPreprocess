#pragma once
#include <string>
#include <vector>

class ITokeniser
{
public:
	virtual void Partition(std::string input,std::vector<std::string>& retWords) = 0;//∑÷¥ À„∑®
};
