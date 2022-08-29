#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

class KeepDataGroup {

private:
	unsigned char SINum;

	unsigned char dataGroupNum;

	unsigned char dataPacketNum;

	unsigned char lastPacketNum;

public:

	int getPacketFlag(struct prefixData &prefix);

	int addDataBlock(unsigned char SINum, unsigned char dataGroupNum, unsigned char dataPacketNum, unsigned char dataBlock[20], struct prefixData &prefix);

	void getDataGroup(unsigned char &SINum, unsigned char &dataGroupNum, unsigned char &lastPacketNum);

	void clearDataGroup();

};


//	lŸŒ³”z—ñ(vector)

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag

//vector<vector<—v‘f‚ÌŒ^>> •Ï”–¼(—v‘f”1, vector<—v‘f‚ÌŒ^>(—v‘f”2, ‰Šú’l));
std::vector<std::vector<std::vector<std::vector<unsigned char>>>> keepDataGroup(13, std::vector<std::vector<std::vector<unsigned char>>>(1, std::vector<std::vector<unsigned char>>(1, std::vector<unsigned char>(22))));

