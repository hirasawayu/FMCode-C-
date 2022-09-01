#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

using std::vector;

class KeepDataGroup {

private:

	//�Ō�̃p�P�b�g�f�[�^�����邩�̃t���O
	unsigned char lastPacketFlag = 0;

	unsigned char SINum = 0;
	unsigned int dataGroupNum = 0;

	unsigned char pushNum = 0;
	unsigned char packetNum = 0;
	unsigned char packetSize = 0;

	

public:

	int getPacketFlag(union PrefixUnion* prefixUnion);

	int addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock);

	void getDataGroup(vector<unsigned char>& dataGroup, union PrefixUnion* prefixUnion);

	void clearDataGroup();

};

//dataGroup�̔ԍ����L�^����\����
struct DataGroupCheck
{
	unsigned char num;
	unsigned int checkedGroupNum;
};