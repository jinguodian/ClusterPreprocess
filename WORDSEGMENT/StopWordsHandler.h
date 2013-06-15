#pragma once
#include <vector>
#include <string>
using std::string;
using std::vector;

// ”√”⁄“∆≥˝Õ£÷π¥ 
class StopWordsHandler
{
private:
	StopWordsHandler(void);

public:
	static StopWordsHandler* GetInstance();
	bool IsStopWord(string& str);

private:
	vector<string> m_vecStopWords;
	static StopWordsHandler* m_pInstance;
	class SingleGCStopWord
	{
	public:
		~SingleGCStopWord(){
			if (StopWordsHandler::m_pInstance != NULL)
			{
				delete StopWordsHandler::m_pInstance;
			}
		}
	};
	static SingleGCStopWord gcsw;
};

