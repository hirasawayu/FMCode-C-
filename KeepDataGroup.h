#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

using std::vector;

class KeepDataGroup {

private:

	

public:

	int getPacketFlag(union PrefixUnion* prefixUnion);

	int addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock);

	void getDataGroup(union PrefixUnion** prefixUnion);

	void clearDataGroup();

};


//	四次元配列(vector)

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag

//vector<vector<要素の型>> 変数名(要素数1, vector<要素の型>(要素数2, 初期値));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(13, vector<vector<vector<unsigned char>>>(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22))));

//dataGroupの番号を記録する構造体
struct DataGroupCheck
{
	unsigned char num;
	unsigned int checkedGroupNum;
};