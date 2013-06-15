#pragma once

#include "ITokeniser.h"
#include <map>
using namespace std;
class TFIDFMeasure
{
private:
	vector<string> m_vecDocs;//�ĵ����ϣ�ÿһ���ַ�������һ���ĵ�
	long m_nDocCount;
	vector<string> m_vecKeyWords;//���ʼ���
	vector<long> m_vecKeyDF;
	ITokeniser* _tokenizer;//�ִ���
	map<string,vector<pair<long,int> > > mapWordsBag;
	vector<pair<int,int> > m_vecMaxTFADF;
	vector<vector<double>> m_vec2DVSM;
	vector<streamoff> m_vecFileOFF;
public:
	TFIDFMeasure(ITokeniser* tokeniser);
	int term_df[2];  //DF��ֵ
	~TFIDFMeasure(void);
	bool Init(vector<string>& vecDoc);//��ʼ��TF-IDF������
	bool Init(const char* pFilePath);
	void SaveArff(const char*);
	void SaveSimpleText(const char* );
	void SaveOFFText(const char*);
private:
	void GetMaxTF();
	void MakeWordBag(const string&,long);
	void FilterKeyWords();
	void GenerateTFIDF();//�����Ƶ
	void Normalization(vector<double>&);//����ʵ�Ȩ�� 
	void WriteArffHead(const char*);
};
