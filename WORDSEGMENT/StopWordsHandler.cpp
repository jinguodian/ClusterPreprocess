#include "StopWordsHandler.h"
#include <fstream>
#include <algorithm>

StopWordsHandler::StopWordsHandler(void)
{
	string tempStr;
	std::ifstream inFile("..\\WORDSEGMENTS\\dic\\stop.dic");
	while( getline(inFile,tempStr) )
	{    
		m_vecStopWords.push_back(tempStr);
	}
	printf("ͣ�ôʹ�����ϣ�\n");
}


StopWordsHandler* StopWordsHandler::m_pInstance = NULL;

StopWordsHandler* StopWordsHandler::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new StopWordsHandler();
	}
	return m_pInstance;
}


bool StopWordsHandler::IsStopWord(string& str)
{
	std::transform(str.begin(),str.end(),str.begin(),tolower);//Сд��
	return std::find(m_vecStopWords.begin(),m_vecStopWords.end(),str) != m_vecStopWords.end();
}

