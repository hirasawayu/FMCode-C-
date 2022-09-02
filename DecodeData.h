
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>


class DecodeData {

private:


	unsigned char dataPacket[22];

	unsigned char dataBlock[20];

	unsigned char dataBlockD[20];

	unsigned char prefix[4];



	//データグループのバイト数+1
	unsigned char dataGroupLink = 0;

	unsigned char packetFlag = 0;

	//データグループデータのバイト数
	uint32_t dataGroupdataSize = 0;

	unsigned char dataGroupType = 0;

	unsigned char parameter = 0;

	unsigned char channelNum = 0;

	unsigned char pageNum = 0;

	unsigned char macroSign = 0;

	std::string channelName;

	unsigned char prefectureNum = 0;

	std::string prefectureName;

	unsigned char XMapNum = 0;
	
	unsigned char XMeshNum = 0;

	unsigned char YMapNum = 0;

	unsigned char YMeshNum = 0;

	unsigned int meshID = 0;

	unsigned int secondMesh = 0;

	unsigned char timeFlag = 0;

	std::string time;

	std::string linkLayer;

	unsigned char unitTypeNum = 0;

	std::string unitTypeName;

	unsigned char DUL = 0;

	unsigned int unitSize = 0;

	unsigned char segmentTypeNum = 0;

	std::string segmentTypeName;

	unsigned char segmentSize;

	std::vector<unsigned char> segmentData;


public:

	void callDecodeData(unsigned char(&dataPacket)[22]);

	void setPrefix(unsigned char *packetData, union PrefixUnion* prefixUnion);

	unsigned char getDataGroupLink(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getDataGroupType(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getParameter(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	void getChannelInfo(unsigned char& channelNum, std::string& channelName, std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getPageNum(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char  getMacroSign(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getPrefecture(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	void  getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp);

	void  getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned int getMeshID(unsigned char XMapNum, unsigned char YMapNum);

	unsigned int getSecondMesh(unsigned char XMapNum, unsigned char YMapNum, unsigned char XMeshNum, unsigned char YMeshNum);

	unsigned char getTime(std::string& time, std::vector<unsigned char>& dataGroup, uint32_t& bp);

	std::string getLinkLayer(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getUnitType(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getDUL(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned int getUnitSize(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getSegmentType(std::string& segmentTypeName, std::vector<unsigned char>& dataGroup, uint32_t& bp, struct SelfSegmentParameter* selfSegmentParameter, struct OtherSegmentParameter* otherSegmentParameter);

	unsigned char getSegmentSize(std::vector<unsigned char>& dataGroup, uint32_t& bp);

	void getSegmentData(std::vector<unsigned char>& segmentData, std::vector<unsigned char>& dataGroup, uint32_t& bp);

	unsigned char getEndGroupFlag();
};

//プリフィックスの情報を格納する構造体・共用体を作成
struct PrefixData
{
	unsigned int dataPacketNum : 8;
	unsigned int PrePageNum : 8;
	unsigned int PreChannelNum : 8;
	unsigned int SINum : 4;
	unsigned int decodeSign : 1;
	unsigned int endSign : 1;
	unsigned int  updateSign : 2;

	//int dataGroupNum = PreChannelNum * 256 + PrePageNum;
};
//付加情報用
struct PrefixDataD
{
	unsigned int SINum : 4;
	unsigned int decodeSign : 1;
	unsigned int endSign : 1;
	unsigned int updateSign : 2;
	unsigned int dataPacketNum : 4;
	unsigned int dataGroupNum : 4;
	unsigned int non_used : 16;
};

union PrefixUnion
{
	unsigned int prefix = 0;
	struct PrefixData prefixData;
	struct PrefixDataD prefixDataD;
};

struct PrefectureList {
	unsigned char prefectureNum = 0;
	std::string  prefectureName;
};

struct UnitParameter {
	unsigned char unitTypeNum = 0;
	std::string unitTypeName;
};

struct SelfSegmentParameter {
	unsigned char segmentTypeNum = 0;
	std::string segmentTypeName;
};

struct OtherSegmentParameter {
	unsigned char segmentTypeNum = 0;
	std::string segmentTypeName;
};


