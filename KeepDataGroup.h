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


//	�l�����z��(vector)

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag

//vector<vector<�v�f�̌^>> �ϐ���(�v�f��1, vector<�v�f�̌^>(�v�f��2, �����l));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(13, vector<vector<vector<unsigned char>>>(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22))));

//dataGroup�̔ԍ����L�^����\����
struct DataGroupCheck
{
	unsigned char num;
	unsigned int checkedGroupNum;
};