#pragma once
#include "ITokeniser.h"

class Tokeniser : public ITokeniser
{
public:
	Tokeniser(void);
	~Tokeniser(void);
public:
	void Partition(std::string input,std::vector<std::string>& retWords);
private:
	std::locale ll;
};
