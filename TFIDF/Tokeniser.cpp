#include "Tokeniser.h"
#include "StopWordsHandler.h"
#include "NLPIR.h"
#include <sstream>
#include <fstream>
#include <locale>
#include <algorithm>

Tokeniser::Tokeniser(void)
{
	//初始化分词组件
	if(!ICTCLAS_Init("",UTF8_CODE))
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	printf("分词组件初始化成功！\n");
}

Tokeniser::~Tokeniser(void)
{
}

void Tokeniser::Partition(string input,vector<string>& retWords)
{
	std::transform(input.begin(),input.end(),input.begin(),tolower);
	string::iterator pos = input.begin();
	StopWordsHandler* pStopWHand = StopWordsHandler::GetInstance();
	string result(ICTCLAS_ParagraphProcess(input.c_str(),0));
	std::stringstream xx;
	xx<<result;
	string str_temp;
	while(xx>>str_temp) {
		if (!pStopWHand->IsStopWord(str_temp)&&!std::isdigit(str_temp[0],ll)
			&&!std::isalpha(str_temp[0],ll)&&(str_temp.find('@') == string::npos)) {
			//不是停用词则保存
			retWords.push_back(str_temp);//保存分出的单词
		}
	}
}

