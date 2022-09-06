#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

using std::vector;

class KeepDataGroup {

private:

	uint32_t ActualGroupNum = 0;
	uint32_t OrderGroupNum = 0;

	//�t�����ł��邩�̃t���O
	unsigned char segmentFlag = 0;
	//�Ō�̃p�P�b�g�f�[�^�����邩�̃t���O
	unsigned char lastPacketFlag = 0;

	unsigned char packetSize = 0;
	

public:

	int getPacketFlag(union PrefixUnion* prefixUnion);

	int addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock);

	vector<unsigned char> getDataGroup(union PrefixUnion* prefixUnion);

	void clearDataGroup();

};

//dataGroup�̔ԍ����L�^����\����
struct DataGroupCheck
{
	uint32_t orderGroupNum = 0;
	uint32_t actualGroupNum = 0;
};