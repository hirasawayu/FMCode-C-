#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

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
std::vector<std::vector<std::vector<std::vector<unsigned char>>>> keepDataGroup(13, std::vector<std::vector<std::vector<unsigned char>>>(1, std::vector<std::vector<unsigned char>>(1, std::vector<unsigned char>(22))));
