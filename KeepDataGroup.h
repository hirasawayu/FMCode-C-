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

	//付加情報であるかのフラグ
	unsigned char segmentFlag = 0;
	//最後のパケットデータがあるかのフラグ
	unsigned char lastPacketFlag = 0;

	unsigned char packetSize = 0;
	

public:

	int getPacketFlag(union PrefixUnion* prefixUnion);

	int addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock);

	vector<unsigned char> getDataGroup(union PrefixUnion* prefixUnion);

	void clearDataGroup();

};

//dataGroupの番号を記録する構造体
struct DataGroupCheck
{
	uint32_t orderGroupNum = 0;
	uint32_t actualGroupNum = 0;
};