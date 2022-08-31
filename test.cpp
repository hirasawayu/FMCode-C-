#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <stdlib.h>


struct PrefixData
{
	unsigned int dataPacketNum : 8;
	unsigned int PrePageNum : 8;
	unsigned int PreChannelNum : 8;
	unsigned int SINum : 4;
	unsigned int decodeSign : 1;
	unsigned int endSign : 1;
	unsigned int  updateSign : 2;
};

struct PrefixDataD
{
	unsigned int dataPacketNum : 4;
	unsigned int dataGroupNum : 4;
	unsigned int SINum : 4;
	unsigned int decodeSign : 1;
	unsigned int endSign : 1;
	unsigned int  updateSign : 2;
};

union PrefixUnion
{
	
	unsigned int prefix = 0;
	struct PrefixData prefixData;
	struct PrefixDataD prefixDataD;
	unsigned int dataGroupNum = PreChannelNum * 256 + PrePageNum;
};

int main()
{
	union PrefixUnion prefixUnion{};

	unsigned char prefixs[4] = {0b10101010, 0b11110000, 0x02, 0x09};
	for (char i = 0; i < 4; i++) {

		prefixUnion.prefix = (prefixUnion.prefix | prefixs[i]);
		if (i == 3) {
			break;
		}
		prefixUnion.prefix = prefixUnion.prefix << 8;
	}

	//unsigned char PreChannelNum = prefixs[1];
	//unsigned char PrePageNum = prefixs[2];
	//unsigned char dataPacketNum = prefixs[3];


	//memset(prefixUnion.prefix, prefixs[0], 4);

	printf("updateSign: %d\n",prefixUnion.prefixData.updateSign);
	printf("endSign: %d\n", prefixUnion.prefixData.endSign);
	printf("decodeSign: %d\n", prefixUnion.prefixData.decodeSign);
	printf("SINum: %d\n", prefixUnion.prefixData.SINum);
	printf("PreChannelNum: %d\n", prefixUnion.prefixData.PreChannelNum);
	printf("PrePageNum: %d\n", prefixUnion.prefixData.PrePageNum);
	printf("dataPacketNum: %d\n", prefixUnion.prefixData.dataPacketNum);
	printf("dataGroupNum: %d\n\n", prefixUnion.prefixData.dataGroupNum);

	printf("updateSign: %d\n", prefixUnion.prefixDataD.updateSign);
	printf("endSign: %d\n", prefixUnion.prefixDataD.endSign);
	printf("decodeSign: %d\n", prefixUnion.prefixDataD.decodeSign);
	printf("SINum: %d\n", prefixUnion.prefixDataD.SINum);
	printf("dataGroupNum: %d\n", prefixUnion.prefixDataD.dataGroupNum);
	printf("dataPacketNum: %d\n", prefixUnion.prefixDataD.dataPacketNum);

	std::cout << std::bitset<32>(prefixUnion.prefix) << std::endl;

	return 0;
}
