
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>


class DecodeData {

private:


	unsigned char dataPacket[22];

	unsigned char dataBlock[18];

	unsigned char prefix[4];



	//データグループのバイト数+1
	unsigned char dataGroupLink;

	unsigned char packetFlag;

	//データグループデータのバイト数
	int dataGroupdataSize;

	unsigned char dataGroupType;

	unsigned char parameter;

	unsigned char channelNum;

	unsigned char pageNum;

	std::string channelName;

	unsigned char macroSign;

	unsigned char prefectureNum;

	std::string prefectureName;

	unsigned char XMapNum;
	
	unsigned char XMeshNum;

	unsigned char YMapNum;

	unsigned char YMeshNum;

	unsigned int meshID;

	unsigned int secondMesh;

	unsigned char timeFlag;

	std::string time;

	std::string linkLayer;

	unsigned char unitTypeNum;

	std::string unitTypeName;

	unsigned char DUL;

	unsigned int unitSize;

	unsigned char segmentTypeNum;

	std::string segmentTypeName;

	unsigned char segmentSize;

	std::vector<unsigned char> dataGroup[];


public:

	void callDecodeData(struct PrefixData& prefixData, struct PrefectureList& prefectureList, struct UnitParameter unitParameter);

	unsigned char bitreverse(unsigned char packetData);

	void setPrefix(unsigned char *packetData);

	void createDataGroup(std::vector<unsigned char>& dataGroup);

	unsigned char getDataGroupLink(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getDataGroupType(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getParameter(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	void getChannelInfo(unsigned char& channelNum, std::string& channelName, std::vector<unsigned char>& dataGroup, unsigned char& bp);

	//unsigned char getUpdateNum(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getPageNum(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char  getMacroSign(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getPrefecture(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	void  getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp);

	void  getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned int getMeshID(unsigned char XMapNum, unsigned char YMapNum);

	unsigned int getSecondMesh(unsigned char XMapNum, unsigned char YMapNum, unsigned char XMeshNum, unsigned char YMeshNum);

	unsigned char getTime(std::string& time, std::vector<unsigned char>& dataGroup, unsigned char& bp);

	std::string getLinkLayer(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getUnitType(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getDUL(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned int getUnitSize(std::vector<unsigned char>& dataGroup, unsigned char& bp);

	unsigned char getSegmentType(std::string& segmentTypeName, std::vector<unsigned char>& dataGroup, unsigned char& bp, struct SelfSegmentParameter* selfSegmentParameter, struct OtherSegmentParameter* otherSegmentParameter);

	unsigned char getSegmentSize();

	

	unsigned char getEndGroupFlag();
};

//プリフィックスの情報を格納する構造体・共用体を作成
struct prefixData
{
	unsigned char SINum : 4;
	unsigned char decodeSign : 1;
	unsigned char endSign : 1;
	unsigned char updateSign : 2;
	unsigned char dataGroupNum : 16;
	unsigned char dataPacketNum : 8;

}prefixData;

union
{
	unsigned char prefix[4];
	struct prefixData prefixData;
} prefixUnion;

struct PrefectureList {
	unsigned char prefectureNum;
	std::string  prefectureName;
};

struct UnitParameter {
	unsigned char unitTypeNum;
	std::string unitTypeName;
};

struct SelfSegmentParameter {
	unsigned char segmentTypeNum;
	std::string segmentTypeName;
};

struct OtherSegmentParameter {
	unsigned char segmentTypeNum;
	std::string segmentTypeName;
};


