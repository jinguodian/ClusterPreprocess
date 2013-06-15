#pragma once

#include "ITokeniser.h"
#include <map>
using namespace std;
class TFIDFMeasure
{
private:
	vector<string> m_vecDocs;//文档集合，每一行字符串代表一份文档
	long m_nDocCount;
	vector<string> m_vecKeyWords;//单词集合
	vector<long> m_vecKeyDF;
	ITokeniser* _tokenizer;//分词器
	map<string,vector<pair<long,int> > > mapWordsBag;
	vector<pair<int,int> > m_vecMaxTFADF;
	vector<vector<double>> m_vec2DVSM;
	vector<streamoff> m_vecFileOFF;
public:
	TFIDFMeasure(ITokeniser* tokeniser);
	int term_df[2];  //DF阀值
	~TFIDFMeasure(void);
	bool Init(vector<string>& vecDoc);//初始化TF-IDF计算器
	bool Init(const char* pFilePath);
	void SaveArff(const char*);
	void SaveSimpleText(const char* );
	void SaveOFFText(const char*);
private:
	void GetMaxTF();
	void MakeWordBag(const string&,long);
	void FilterKeyWords();
	void GenerateTFIDF();//计算词频
	void Normalization(vector<double>&);//计算词的权重 
	void WriteArffHead(const char*);
};
