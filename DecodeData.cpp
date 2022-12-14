#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>
#include <cmath>

#include "DecodeData.h"
#include "KeepDataGroup.h"
#include "DataBase.h"

//KeepDataGroupΜIuWFNgπΆ¬
KeepDataGroup keep;
DataBase data;
PrefixUnion prefixUnion;


struct ParameterList parameterList[12]{
	{0, "ΤgΗA"},
	{1, "ΤgΗB"},
	{2, "y[Wf[^A"},
	{3, "y[Wf[^B"},
	{4, "Τg€Κ}Nf[^A"},
	{5, "Τg€Κ}Nf[^B"},
	{6, "p±f[^"},
	{7, "Τgυψf[^"},
	{8, "ΤgΗA"},
	{9, "ΤgΗB"},
	{10, "y[Wf[^A"},
	{11, "y[Wf[^B"},
};

struct PrefectureList prefectureList[47]
{
	{1, "(kCΉ)"},
	{2, "(ΒX§)"},
	{3, "(βθ§)"},
	{4, "({ι§)"},
	{5, "(Hc§)"},
	{6, "(R`§)"},
	{7, "(§)"},
	{8, "(οι§)"},
	{9, "(ΘΨ§)"},
	{10, "(Qn§)"},
	{11, "(ιΚ§)"},
	{12, "(ηt§)"},
	{13, "(s)"},
	{14, "(_ήμ§)"},
	{15, "(V§)"},
	{16, "(xR§)"},
	{17, "(Ξμ§)"},
	{18, "(δ§)"},
	{19, "(R§)"},
	{20, "(·μ§)"},
	{21, "(ς§)"},
	{22, "(Γͺ§)"},
	{23, "(€m§)"},
	{24, "(Od§)"},
	{25, "( κ§)"},
	{26, "(s{)"},
	{27, "(εγ{)"},
	{28, "(ΊΙ§)"},
	{29, "(ήΗ§)"},
	{30, "(aΜR§)"},
	{31, "(Ήζ§)"},
	{32, "(ͺ§)"},
	{33, "(ͺR§)"},
	{34, "(L§)"},
	{35, "(Rϋ§)"},
	{36, "(Ώ§)"},
	{37, "(μ§)"},
	{38, "(€Q§)"},
	{39, "(m§)"},
	{40, "(ͺ§)"},
	{41, "(²κ§)"},
	{42, "(·θ§)"},
	{43, "(F{§)"},
	{44, "(εͺ§)"},
	{45, "({θ§)"},
	{46, "(­§)"},
	{47, "(«κ§)"},
};

struct UnitParameter unitParameter[28]{
	{0x20, "({Ά)"},
	{0x24, "(wb_Ά)"},
	{0x26, "(rf[^)"},
	{0x28, "(WIgbN)"},
	{0x29, "(Τg€Κ}N)"},
	{0x30, "(1oCgDRCS)"},
	{0x31, "(2oCgDRCS)"},
	{0x34, "(J[}bv)"},
	{0x36, "(Iπ§δ)"},
	{0x38, "(κwtHgOtBbN)"},
	{0x3B, "(p±)"},
	{0x3C, "(Τgυψ)"},
	{0x3E, "(_~[)"},
	{0x3F, "(lbg[N^p)"},
	{0x40, "(aΨE·sΤ)"},
	{0x41, "(K§EΜ)"},
	{0x42, "(Τκ)"},
	{0x43, "(ζΤ·sΤ)"},
	{0x44, "(xaΨξρ)"},
	{0x45, "(g£aΨE·sΤ)"},
	{0x46, "(g£ΫK§)"},
	{0x47, "({έξρ)"},
	{0x48, "(Ω}ξρ)"},
	{0x49, "(bZ[Wξρ)"},
	{0x50, "(XNuξρ)"},
	{0x60, "(Τg§δ)"},
	{0x61, "(VXe§δ)"},
	{0x62, "(VXef[^)"},
};

struct SelfSegmentParameter selfSegmentParameter[8]{
	{0x01, "(ϊΗ―Κ)"},
	{0x03, "(ΤgJn?¬Τ)"},
	{0x04, "(πΚEΩ}ξρtO)"},
	{0x05, "(Τgξρ)"},
	{0x06, "(eΗΌ)"},
	{0x08, "(pϊΗΌ)"},
	{0x0A, "(γΦόg)"},
	{0x0C, "(JeSΌΜΔθ`)"},
};

struct OtherSegmentParameter otherSegmentParameter[11]{
	{0x01, "(ϊΗ―Κ)"},
	{0x03, "(ΤgJn?¬Τ)"},
	{0x04, "(πΚEΩ}ξρtO)"},
	{0x05, "(Τgξρ)"},
	{0x06, "(ξ²ϊΗΌ)"},
	{0x07, "(ξ²ϊΗΌ)"},
	{0x08, "(pϊΗΌ)"},
	{0x09, "(pϊΗΌ)"},
	{0x0A, "(γΦόg)"},
	{0x0B, "(γΦόg)"},
	{0x0C, "(JeSΌΜΔθ`)"},
};

void DecodeData::setPrefix(unsigned char* packetData, union PrefixUnion* prefixUnion) {

	prefixUnion->prefix = 0;
	//tΑξρΜκ
	//0000 1111π}XN
	if ((packetData[0] & 0x0f) == 0x0D) {
		//pΣ΅½zρΙlπRs[

		for (char i = 0; i < 2; i++) {

			prefixUnion->prefix = (prefixUnion->prefix | packetData[i]);
			if (i == 1) {
				break;
			}
			prefixUnion->prefix = prefixUnion->prefix << 8;
		}
	}

	else {
		for (char i = 0; i < 4; i++) {

			prefixUnion->prefix = (prefixUnion->prefix | packetData[i]);
			if (i == 3) {
				break;
			}
			prefixUnion->prefix = prefixUnion->prefix << 8;
		}
	}
}

unsigned char DecodeData::getDataGroupLink(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//1000 0000πμp΅½}XN
	if ((dataGroup[bp] & 0x80) == 0x80) {
		return 1;
	}
	else {
		return 0;
	}
}

unsigned char  DecodeData::getDataGroupType(std::vector<unsigned char>& dataGroup, uint32_t& bp) {

	if (dataGroup[bp] == 0x1E) {
		return 0;
	}
	else if (dataGroup[bp] == 0x1F) {
		return 1;
	}
	//G[
	else {
		printf("error in getDataGroupType\n");
		return 0;
	}
}

unsigned char DecodeData::getParameter(std::string& parameterName, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0000 1111πμp΅½}XN
	unsigned char para = (dataGroup[bp] & 0x0F);
	for (int i = 0; i < 12; i++) {
		if (para == parameterList[i].parameterNum) {
			parameterName = parameterList[i].parameterName;
		}
	}
	return para;
}

void  DecodeData::getChannelInfo(unsigned char& channelNum, std::string& channelName, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	channelNum = dataGroup[bp];
	if (channelNum == 0) {
		channelName = "(Ϊ/ΕΜζΚ)";
	}
	else if (channelNum >= 1 && channelNum < 20) {
		channelName = "(j[X)";
	}
	else if (channelNum >= 20 && channelNum < 40) {
		channelName = "(κΚξρΤg)";
	}
	else if (channelNum >= 40 && channelNum < 80) {
		channelName = "(πΚξρ)";
	}
	else if (channelNum >= 80 && channelNum < 100) {
		channelName = "(Τgξρ)";
	}
	else if (channelNum >= 100 && channelNum < 150) {
		channelName = "(CΫξρ)";
	}
	else if (channelNum >= 150 && channelNum < 250) {
		channelName = "(»ΜΌ)";
	}
	return;
}

//ΰeXVΝsvH
//unsigned char  DecodeData::getUpdateNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
//}
unsigned char  DecodeData::getPageNum(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0011 1111πμp΅½}XN
	unsigned char page = (dataGroup[bp] & 0x3F);
	return page;
}

unsigned char  DecodeData::getMacroSign(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//4ErbgVtg(1111 0000)
	unsigned char macro = (dataGroup[bp] >> 4);
	return macro;
}

unsigned char  DecodeData::getPrefecture(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0011 1111πμp΅½}XN
	unsigned char prefec = (dataGroup[bp] & 0x3F);
	return prefec;
}

void  DecodeData::getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//3ErbgVtg(1111 1000)
	XMapNum = (dataGroup[bp] >> 3);

	//0000 0111π}XN
	XMeshNum = (dataGroup[bp] & 0x07);

	return;
}

void  DecodeData::getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//3ErbgVtg(1111 1000)
	YMapNum = (dataGroup[bp] >> 3);

	//0000 0111π}XN
	YMeshNum = (dataGroup[bp] & 0x07);

	return;
}

//caution (XMapNum YMapNum)
unsigned int DecodeData::getMeshID(unsigned char XMapNum, unsigned char YMapNum) {
	unsigned int meshID = XMapNum * 32 + YMapNum;
	return meshID;
}

unsigned int DecodeData::getSecondMesh(unsigned char XMapNum, unsigned char YMapNum, unsigned char XMeshNum, unsigned char YMeshNum) {
	unsigned int secondMesh = (YMapNum + 37) * 10000 + (XMapNum + 22) * 100 + YMeshNum * 10 + XMeshNum;
	return secondMesh;
}

unsigned char  DecodeData::getTime(std::string& time, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0000 1000π}XN
	unsigned char timeFlag = (dataGroup[bp] & 0x08);
	if (timeFlag == 1) {
		unsigned char hour = ((dataGroup[bp] & 0x07) << 3);

		bp++;
		hour += ((dataGroup[bp] & 0xC0) >> 6);
		unsigned char minute = (dataGroup[bp] & 0x3F);

		time = hour + ":" + minute;
	}
	return timeFlag;
}

std::string DecodeData::getLinkLayer(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//1100 0000π}XN
	unsigned char linkLayerNum = ((dataGroup[bp] & 0xC0) >> 6);
	switch (linkLayerNum) {
	case 1:
		return "Lζ";
	case 2:
		return "ζ";
	case 3:
		return "·ζ";
	};
}

unsigned char DecodeData::getUnitType(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	unsigned char unitTypeNum = dataGroup[bp];
	return unitTypeNum;
}

unsigned char DecodeData::getDUL(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//1000 0000π}XN
	if ((dataGroup[bp] & 0x80) == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

unsigned int  DecodeData::getUnitSize(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0111 1111π}XN
	unsigned int unitSize = (dataGroup[bp] & 0x7F) * 16 * 16;
	bp++;
	unitSize += dataGroup[bp];

	return unitSize;
};

unsigned char  DecodeData::getSegmentType(std::string& segmentTypeName, std::vector<unsigned char>& dataGroup, uint32_t& bp, struct SelfSegmentParameter* selfSegmentParameter, struct OtherSegmentParameter* otherSegmentParameter) {
	//1111 0000π}XN
	unsigned char segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
	//©ΗpΜκ
	if (segmentTypeNum != 0x0E) {
		segmentTypeName = selfSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
	//ΌΗpΜκ
	else {
		bp++;
		segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
		segmentTypeName = otherSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
}

unsigned char  DecodeData::getSegmentSize(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0000 1111π}XN
	unsigned char segmentSize = (dataGroup[bp] & 0x0F);
	if (segmentSize != 0x0F) {
		return segmentSize;
	}
	else {
		bp++;
		segmentSize = dataGroup[bp];
		return segmentSize;
	}
}

void DecodeData::getSegmentData(std::vector<unsigned char>& segmentData, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	for (int i = 0; i < segmentSize; i++) {
		segmentData.push_back (dataGroup[bp]);
		bp++;
	}
	return;
}

//SIΜtest
//unsigned char DecodeData::testSI();

//fR[hNXΜmain
void  DecodeData::callDecodeData(unsigned char(&dataPacket)[22])
{
	
	setPrefix(dataPacket, &prefixUnion);


	packetFlag = keep.getPacketFlag(&prefixUnion);

	//pPbgΤπJEg
	packetCount++;
	printf("packetCount: %04d\n", packetCount);

	unsigned char ret = 0;

	//wθΜΚuΙpPbgf[^ͺόΑΔ’Θ’κ
	if (packetFlag == 0) {
		if (((prefixUnion.prefix >> 24) & 0x0f) != 0x0D) {
			for (int i = 0; i < 18; i++) {
				dataBlock[i] = dataPacket[i + 4];
			}
		}
		else if (((prefixUnion.prefix >> 24) & 0x0f) == 0x0D) {
			for (int i = 0; i < 20; i++) {
				dataBlock[i] = dataPacket[i + 4];
			}
		}

		//KeepDataBlockNXΜwθΜΚuΙf[^ubNπi[·ι
		ret = keep.addDataBlock(&prefixUnion, dataBlock);
	}

	//f[^ubNͺ΅€άΕ[v
	if (ret != 1) {
		return;
	}

	//KwSΜf[^O[v
	std::vector<unsigned char> dataGroup;
	dataGroup = keep.getDataGroup(&prefixUnion);

	//gp΅Θ’ππϊ
	//keep.clearDataGroup();

	//QΖ·ιoCgΚu(BytePlace)πL―
	uint32_t bp = 0;

	unsigned char endLoopFlag = 0;
	while (1) {
		while (1) {

			//ZOgf[^Μκ
			if (((prefixUnion.prefix >> 24) & 0x0f) == 0x0D) {
				//(n)ZOg―ΚζΎ
				segmentTypeNum = getSegmentType(segmentTypeName, dataGroup, bp, selfSegmentParameter, otherSegmentParameter);
				data.setData(18, 0, 0, segmentTypeName);

				//ZOgTCYζΎ
				segmentSize = getSegmentSize(dataGroup, bp);
				//pPbgΤζΎ
				data.setData(19, 0, packetCount, 0);
				
				//ZOgf[^ζΎ
				bp++;
				getSegmentData(segmentData, dataGroup, bp);

				return;
			}

			//BD2 DGLζΎ
			bp++;
			dataGroupLink = getDataGroupLink(dataGroup, bp);

			//DB(n), +2@O[v\¬ζΎ
			bp += 2;
			dataGroupType = getDataGroupType(dataGroup, bp);

			//DB(n+1)@p[^ζΎ
			bp++;
			//RS(f[^wb_)Μκ
			if (dataGroupType == 0) {

				parameter = getParameter(parameterName, dataGroup, bp);
				//delete
				printf("parameter: %x\n", parameter);
				data.setData(2, 0, 0, 0);
				data.setData(3, 0, 0, parameterName);

				
				std::cout << parameterName << std::endl;

				//DB(n+2)@ΤgΤζΎ
				bp++;
				if ((parameter >= 0 && parameter <= 4) || (parameter >= 8 && parameter <= 11)) {
					getChannelInfo(channelNum, channelName, dataGroup, bp);
					data.setData(4, channelNum, 0, 0);
					data.setData(5, 0, 0, channelName);
				}
				else if (parameter == 6) {
					bp = 0;
					break;
				}

				//DB(n+3) ΰeXVAy[W/y[WΤζΎ
				bp++;
				if ((parameter >= 0 && parameter <= 4) || (parameter >= 8 && parameter <= 11)) {
					pageNum = getPageNum(dataGroup, bp);
					data.setData(6, pageNum, 0, 0);
				}



				//Τg€Κ}NπΝ
				//DB(n+4) Τg€Κ}NWζΎ
				
				if (parameter == 4 || parameter == 5) {
					bp++;
					macroSign = getMacroSign(dataGroup, bp);

					//}NWͺ1ΰ΅­Ν2oCgΜͺς
					if (macroSign == 2) {
						bp += 3;
					}
					if (macroSign == 3) {
						bp += 2;
					}

					//\¬AΝ±±ΕIΉ
					if (parameter == 4) {
						break;
					}

					//(n+6/7) sΉ{§ζΎ
					prefectureNum = getPrefecture(dataGroup, bp);
					prefectureName = prefectureList[prefectureNum].prefectureName;

					//Τg€Κ}NΝ±±ΕIΉ
					bp += 7;
					break;
				}

				//Τgf[^wb_Μκ
				if (parameter == 0 || parameter == 1) {
					bp += 3;
				}
				//y[Wf[^wb_Μκ
				if (parameter == 2 || parameter == 3) {
					bp +=4;
				}
				//\¬AΝ±±ΕIΉ
				if (parameter == 0 || parameter == 2) {
					break;
				}

				//sΉ{§ζΎ
				prefectureNum = getPrefecture(dataGroup, bp);
				prefectureName = prefectureList[prefectureNum].prefectureName;
				data.setData(7, prefectureNum, 0, 0);
				data.setData(8, 0, 0, prefectureName);

				//(n += 2)XΐW@n}ΤAρbVζΎ
				bp += 2;
				getXMapAddress(XMapNum, XMeshNum, dataGroup, bp);
				data.setData(9, XMapNum, 0, 0);

				//(n += 1)YΐW@n}ΤAρbVζΎ
				bp++;
				getYMapAddress(YMapNum, YMeshNum, dataGroup, bp);
				data.setData(10, YMapNum, 0, 0);

				//MeshIDAρbVζΎ
				meshID = getMeshID(XMapNum, YMapNum);
				data.setData(11, 0, meshID, 0);

				secondMesh = getSecondMesh(XMapNum, YMapNum, XMeshNum, YMeshNum);
				data.setData(12, 0, secondMesh, 0);

				//(n += 2)Τgf[^wb_Ν±±ΕIΉ
				bp += 2;
				
				if (parameter == 1) {
					break;
				}
				//ξρρζΎ
				timeFlag = getTime(time, dataGroup, bp);
				data.setData(13, 0, 0, time);

				//(n += 1)NCζΎ
				bp++;
				linkLayer = getLinkLayer(dataGroup, bp);
				data.setData(14, 0, 0, linkLayer);

				//(n += 2)y[Wf[^wb_IΉ
				bp += 2;
				break;
			}
		}

		//·³²?NULLπXLbv
		while (dataGroup[bp] == 0x00) {
			bp++;
		}

		if (dataGroup[bp] == 0x03 || dataGroup[bp] == 0x04 || dataGroup[bp] == 0x17) {
			return;
		}

		while (1) {

			//US(jbgf[^)Μκ
			dataGroupType = getDataGroupType(dataGroup, bp);
			if (dataGroupType == 1) {
				//(n += 1)f[^jbg^CvζΎ
				
				bp += 1;
				unitTypeNum = getUnitType(dataGroup, bp);
				for (int i = 0; i < 28; i++) {
					if (unitTypeNum == unitParameter->unitTypeNum) {
						unitTypeName = unitParameter[i].unitTypeName;
					}
				}
				data.setData(15, unitTypeNum, 0, 0);
				data.setData(16, 0, 0, unitTypeName);
				//delete
				printf("UnitTypeNum: %x\n", unitTypeNum);

				//(n += 1)DULζΎ
				bp += 1;
				DUL = getDUL(dataGroup, bp);

				//jbgTCYζΎ
				unitSize = getUnitSize(dataGroup, bp);
				data.setData(17, 0, unitSize, 0);

				//jbgTCYͺXLbv
				bp += (unitSize + 1);
				if (dataGroup[bp] != 0x1F) {
					break;
				}
			}

		}
		//·³²?NULLπXLbv
		while (dataGroup[bp] == 0x00) {
			bp++;
		}

		if (dataGroup[bp] == 0x03 || dataGroup[bp] == 0x04 || dataGroup[bp] == 0x17) {
			return;
		}
	}
}

