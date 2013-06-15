// ClusterPreprocess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <fstream>
#include "Tokeniser.h"
#include "TFIDFMeasure.h"
#pragma comment(lib,"NLPIR.lib") 

int main(int argc, char* argv[])
{
// #ifdef	_DEBUG
// 	string dataDir = "E:\\Cluster\\";
// 	string inFile = dataDir + "new.txt";
// 	string outArff = dataDir + "new.arff";
// #else
	if (argc < 3)
	{
		printf("the count of program argument must more than 2\n");
		return -1;
	}
	string inFile(argv[1]);
	string outArff(argv[2]);
/*#endif*/
	ITokeniser * ken = new Tokeniser();
	TFIDFMeasure tfidf(ken);
	tfidf.term_df[0] = 2;
	tfidf.term_df[1] = 0;
	tfidf.Init(inFile.c_str());
	tfidf.SaveArff(outArff.c_str());
	printf("����Ԥ���������� \n");
	
	return 0;
}

