#include "TFIDFMeasure.h"
#include <limits>
#include <cmath>
#include <iterator>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

TFIDFMeasure::~TFIDFMeasure(void)
{
	//destroy
	if (this->_tokenizer!=NULL)
	{
		delete _tokenizer;
		_tokenizer = NULL;
	}
}

TFIDFMeasure::TFIDFMeasure(ITokeniser* tokeniser)
{
	term_df[0] = 0;
	term_df[1] = 0;
	_tokenizer = tokeniser;
}

bool TFIDFMeasure::Init(vector<string> &vecDoc)
{

	for (vector<string>::iterator it = vecDoc.begin(); it != vecDoc.end(); ++it)
	{
		vector<string>::difference_type doc_index = it - vecDoc.begin();
		MakeWordBag(*it,doc_index);
	}
	m_nDocCount = vecDoc.size();
	if (term_df[0] == 0)
	{
		term_df[0] = (int)sqrt(sqrt((double)m_nDocCount));
	}
	if (term_df[1] == 0)
	{
		term_df[1] = (int)sqrt((double)m_nDocCount)*2;
	}
	FilterKeyWords();

	this->GenerateTFIDF();
	return true;
}

bool TFIDFMeasure::Init(const char* pFilePath)
{
	ifstream inFile(pFilePath);
	if (!inFile.bad())
	{
		printf("开始建立词袋子模型...\n");
		string xx;
		long docindex(0);
	
		while(getline(inFile,xx)){
			MakeWordBag(xx,docindex);
			m_vecFileOFF.push_back(inFile.tellg());
			++ docindex;
		}
		if (xx.empty())
		{
			return false;
		}
		m_nDocCount = docindex;
		printf("词袋子模型构建完成...\n共有%d条文本\n",m_nDocCount);
	}
	else{
		return false;
	}
	inFile.close();
	
	if (term_df[0] == 0)
	{
		term_df[0] = (int)sqrt(sqrt((double)m_nDocCount));
	}
	if (term_df[1] == 0)
	{
		term_df[1] = m_nDocCount/2;
	}
	this->FilterKeyWords();
	
	printf("关键词选择完毕...\n");

	ofstream outKey("key");
	for (int i = 0; i < m_vecKeyWords.size(); ++i)
	{
		outKey<<m_vecKeyWords[i]<<" "<<m_vecKeyDF[i]<<endl;
	}
	
	outKey.close();
	this->GetMaxTF();
	printf("开始文档向量化...\n");
	this->GenerateTFIDF();
	printf("向量空间模型建立完成...\n");

	return true;
}

void TFIDFMeasure::GetMaxTF()
{
	for(vector<string>::iterator it = m_vecKeyWords.begin(); it != m_vecKeyWords.end();it++)
	{  
		int DF = mapWordsBag[*it].size();
		int maxTF=0;
		for_each(mapWordsBag[*it].begin(),mapWordsBag[*it].end(),[&maxTF](pair<long,int>& _val)
		{
			maxTF = maxTF>=_val.second ? maxTF : _val.second;
		}
		);
		m_vecMaxTFADF.push_back(make_pair(maxTF,DF));
	}
}

void TFIDFMeasure::MakeWordBag(const string & sDoc,long doc_index)
{
	vector<string> vecSTemp;
	_tokenizer->Partition(sDoc,vecSTemp);
	
	map<string,int>::iterator mapsiIt;
	for (vector<string>::iterator tmpit = vecSTemp.begin(); tmpit != vecSTemp.end(); ++tmpit)
	{

		if (!mapWordsBag[*tmpit].empty())
		{
			vector<pair<long,int> >::iterator vecpIt = find_if(mapWordsBag[*tmpit].begin(),mapWordsBag[*tmpit].end(),
				[&doc_index](const pair<long,int>& pairtmp){
					return doc_index == pairtmp.first;
			}
			);
			if (vecpIt == mapWordsBag[*tmpit].end())
			{
				mapWordsBag[*tmpit].push_back(make_pair(doc_index,1));
			}
			else{
				++vecpIt->second;
			}
		}
		else
		{
			mapWordsBag[*tmpit].push_back(make_pair(doc_index,1));
		}
	}
}

void TFIDFMeasure::FilterKeyWords()
{
	typedef map<string,vector<pair<int,int> > > mapSVec;

	for_each(mapWordsBag.begin(),mapWordsBag.end(),
		[this](pair<string,vector<pair<long,int>>> _val) {
			if (_val.second.size() > term_df[0] && _val.second.size() < term_df[1])
			{
				m_vecKeyWords.push_back(_val.first);
				m_vecKeyDF.push_back(_val.second.size());
			}
	}
	);
}

void TFIDFMeasure::GenerateTFIDF()
{
	long wordcount = m_vecKeyWords.size();
	m_vec2DVSM.reserve(m_nDocCount);
	for_each(m_vec2DVSM.begin(),m_vec2DVSM.end(),
		[&wordcount](vector<double>& _val){
			_val.reserve(wordcount);
	}
		);

	for (long i = 0; i < m_nDocCount ; ++i)
	{
		vector<double> vecPTmpDocVSM;
		int max_Tf(0);
		for (long j = 0; j < wordcount; ++j)
		{
			map<string,vector<pair<long,int>>>::iterator mapsvIter = mapWordsBag.find(m_vecKeyWords[j]);
			vector<pair<long,int>>::iterator vecpIt = 
				find_if(mapsvIter->second.begin(),mapsvIter->second.end(),
				[&i](pair<long,int>& _val){
					return _val.first == i;
			}
			);
			int termFe(0);
			if (vecpIt != mapsvIter->second.end())
			{
				termFe = vecpIt->second;
				max_Tf = termFe > max_Tf ? termFe : max_Tf;
			}
		//	double tFe = ((double)termFe)/m_vecMaxTFADF[j].first;
			double tFe = 0.5 + 0.5*((double)termFe/m_vecMaxTFADF[j].first);

			double termDF = log(m_nDocCount/(double)m_vecMaxTFADF[j].second);
			
			vecPTmpDocVSM.push_back(tFe*termDF);
			
		}
		
	//	if (max_Tf > 0)
		Normalization(vecPTmpDocVSM);

		m_vec2DVSM.push_back(vecPTmpDocVSM);
	}
	mapWordsBag.clear();
}

inline bool IsZero(double a){
	return fabs(a) <= std::numeric_limits<double>::epsilon();
}

void TFIDFMeasure::Normalization(vector<double>& vecTFIDF)
{
	double sum(0);
	for_each(vecTFIDF.begin(),vecTFIDF.end(),
		[&sum](double& xx){
		//	sum = sum>=xx ? sum : xx;
			sum += pow(xx,2);
	}
	);
	sum = sqrt(sum);
	for_each(vecTFIDF.begin(),vecTFIDF.end(),
		[&sum](double& xx){
			xx /= sum;
	}
	);
}

void TFIDFMeasure::SaveArff(const char* pFile)
{
	printf("写入ARFF文件...");
	WriteArffHead(pFile);
	ofstream oufile(pFile,std::ios::binary|std::ios::app);
	ostringstream dtos;
	for_each(m_vec2DVSM.begin(),m_vec2DVSM.end(),[&dtos,&oufile](vector<double>& _val){
			for_each(_val.begin(),_val.end(),[&](double& _cVal){
				dtos<<_cVal;
				oufile<<dtos.str()<<",";
				dtos.str("");
			}
			);
			oufile<<endl;
	}
	);
	oufile.close();
	printf("写入成功...\n");
}

void TFIDFMeasure::SaveSimpleText(const char* pFile)
{

	ofstream oufile(pFile,std::ios::binary);
	stringstream dtos;
	oufile<<m_nDocCount<<" "<<m_vecKeyWords.size()<<"\n"<<"\n";
	/*
	for_each(m_vecKeyWords.begin(),m_vecKeyWords.end(),[&](string& _val){
		oufile<<_val<<"\n";
	}
		);
		*/
	for_each(m_vec2DVSM.begin(),m_vec2DVSM.end(),[&dtos,&oufile](vector<double>& _val){
			for_each(_val.begin(),_val.end(),[&](double& _cVal){
				dtos.str("");
				dtos<<_cVal;
				oufile<<dtos.str()<<" ";
			}
			);
			oufile<<endl;
	}
	);
	oufile.close();
}

void TFIDFMeasure::SaveOFFText(const char* pFile){
	ofstream oufile(pFile,std::ios::binary);
	for_each(m_vecFileOFF.begin(),m_vecFileOFF.end(),[&oufile](streamoff &x){
		oufile<<x<<"\n";
	});
	oufile.close();
}

void TFIDFMeasure::WriteArffHead(const char* pFile)
{
    ofstream ofile(pFile,ios::binary);
    ofile<<"@relation aticle"<<endl;
    ofile<<"\n";       
	ostringstream temp;
    for(vector<string>::iterator it = m_vecKeyWords.begin(); it != m_vecKeyWords.end(); it++)
    {
        //string temp="@attribute "+"'"+(*it)+"'"+" real";
 
        temp<<"@attribute ";
        temp<<"'";
       // temp+=*it;
		temp<<distance(m_vecKeyWords.begin(),it)<<"' real";
    
 
        ofile<<temp.str()<<endl;
		temp.seekp(0);
    }
	
    ofile<<"\n"<<endl;
    ofile<<"@data"<<endl;
    ofile.close();
}