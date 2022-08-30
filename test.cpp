#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <stdlib.h>


struct PrefixData
{
	unsigned char updateSign : 2;
	unsigned char endSign : 1;
	unsigned char decodeSign : 1;
	unsigned char SINum : 4;

	//unsigned char PreChannelNum : 8;
	//unsigned char PrePageNum : 8;
	//unsigned char dataPacketNum : 8;

	//int dataGroupNum = PreChannelNum * 256 + PrePageNum;
};

union PrefixUnion
{
	
	unsigned char prefix;
	struct PrefixData prefixData;
};

int main()
{
	union PrefixUnion prefixUnion;;

	unsigned char prefixs = 0xAA;//, 0x01, 0x02, 0x03 };

	prefixUnion.prefix = prefixs; // [0] ;

	//unsigned char PreChannelNum = prefixs[1];
	//unsigned char PrePageNum = prefixs[2];
	//unsigned char dataPacketNum = prefixs[3];


	//memset(prefixUnion.prefix, prefixs[0], 4);

	printf("updateSign: %d\n",prefixUnion.prefixData.updateSign);
	printf("endSign: %d\n", prefixUnion.prefixData.endSign);
	printf("decodeSign: %d\n", prefixUnion.prefixData.decodeSign);
	printf("SINum: %d\n", prefixUnion.prefixData.SINum);
	//printf("PreChannelNum: %d\n", prefixUnion.prefixData.PreChannelNum);
	//printf("PrePageNum: %d\n", prefixUnion.prefixData.PrePageNum);
	//printf("dataPacketNum: %d\n", prefixUnion.prefixData.dataPacketNum);

	return 0;
}
