#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>
#include <cmath>

#include "DecodeData.h"
#include "KeepDataGroup.h"
#include "DataBase.h"

//KeepDataGroupのオブジェクトを生成
KeepDataGroup keep;
DataBase data;
PrefixUnion prefixUnion;


struct ParameterList parameterList[12]{
	{0, "番組管理A"},
	{1, "番組管理B"},
	{2, "ページデータA"},
	{3, "ページデータB"},
	{4, "番組共通マクロデータA"},
	{5, "番組共通マクロデータB"},
	{6, "継続データ"},
	{7, "番組索引データ"},
	{8, "番組管理A"},
	{9, "番組管理B"},
	{10, "ページデータA"},
	{11, "ページデータB"},
};

struct PrefectureList prefectureList[47]
{
	{1, "(北海道)"},
	{2, "(青森県)"},
	{3, "(岩手県)"},
	{4, "(宮城県)"},
	{5, "(秋田県)"},
	{6, "(山形県)"},
	{7, "(福島県)"},
	{8, "(茨城県)"},
	{9, "(栃木県)"},
	{10, "(群馬県)"},
	{11, "(埼玉県)"},
	{12, "(千葉県)"},
	{13, "(東京都)"},
	{14, "(神奈川県)"},
	{15, "(新潟県)"},
	{16, "(富山県)"},
	{17, "(石川県)"},
	{18, "(福井県)"},
	{19, "(山梨県)"},
	{20, "(長野県)"},
	{21, "(岐阜県)"},
	{22, "(静岡県)"},
	{23, "(愛知県)"},
	{24, "(三重県)"},
	{25, "(滋賀県)"},
	{26, "(京都府)"},
	{27, "(大阪府)"},
	{28, "(兵庫県)"},
	{29, "(奈良県)"},
	{30, "(和歌山県)"},
	{31, "(鳥取県)"},
	{32, "(島根県)"},
	{33, "(岡山県)"},
	{34, "(広島県)"},
	{35, "(山口県)"},
	{36, "(徳島県)"},
	{37, "(香川県)"},
	{38, "(愛媛県)"},
	{39, "(高知県)"},
	{40, "(福岡県)"},
	{41, "(佐賀県)"},
	{42, "(長崎県)"},
	{43, "(熊本県)"},
	{44, "(大分県)"},
	{45, "(宮崎県)"},
	{46, "(鹿児島県)"},
	{47, "(沖縄県)"},
};

struct UnitParameter unitParameter[28]{
	{0x20, "(本文)"},
	{0x24, "(ヘッダ文)"},
	{0x26, "(ルビデータ)"},
	{0x28, "(ジオメトリック)"},
	{0x29, "(番組共通マクロ)"},
	{0x30, "(1バイトDRCS)"},
	{0x31, "(2バイトDRCS)"},
	{0x34, "(カラーマップ)"},
	{0x36, "(選択制御)"},
	{0x38, "(一層フォトグラフィック)"},
	{0x3B, "(継続)"},
	{0x3C, "(番組索引)"},
	{0x3E, "(ダミー)"},
	{0x3F, "(ネットワーク運用)"},
	{0x40, "(渋滞・旅行時間)"},
	{0x41, "(規制・事故)"},
	{0x42, "(駐車場)"},
	{0x43, "(区間旅行時間)"},
	{0x44, "(高度渋滞情報)"},
	{0x45, "(拡張渋滞・旅行時間)"},
	{0x46, "(拡張事象規制)"},
	{0x47, "(施設情報)"},
	{0x48, "(緊急情報)"},
	{0x49, "(メッセージ情報)"},
	{0x50, "(スクランブル情報)"},
	{0x60, "(番組制御)"},
	{0x61, "(システム制御)"},
	{0x62, "(システムデータ)"},
};

struct SelfSegmentParameter selfSegmentParameter[8]{
	{0x01, "(放送局識別)"},
	{0x03, "(番組開始編成時間)"},
	{0x04, "(交通・緊急情報フラグ)"},
	{0x05, "(番組情報)"},
	{0x06, "(親局名)"},
	{0x08, "(中継放送局名)"},
	{0x0A, "(代替周波数)"},
	{0x0C, "(カテゴリ名の再定義)"},
};

struct OtherSegmentParameter otherSegmentParameter[11]{
	{0x01, "(放送局識別)"},
	{0x03, "(番組開始編成時間)"},
	{0x04, "(交通・緊急情報フラグ)"},
	{0x05, "(番組情報)"},
	{0x06, "(基幹放送局名)"},
	{0x07, "(基幹放送局名)"},
	{0x08, "(中継放送局名)"},
	{0x09, "(中継放送局名)"},
	{0x0A, "(代替周波数)"},
	{0x0B, "(代替周波数)"},
	{0x0C, "(カテゴリ名の再定義)"},
};

void DecodeData::setPrefix(unsigned char* packetData, union PrefixUnion* prefixUnion) {

	prefixUnion->prefix = 0;
	//付加情報の場合
	//0000 1111をマスク
	if ((packetData[0] & 0x0f) == 0x0D) {
		//用意した配列に値をコピー

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
	//1000 0000を作用したマスク処理
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
	//エラー
	else {
		printf("error in getDataGroupType\n");
		return 0;
	}
}

unsigned char DecodeData::getParameter(std::string& parameterName, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0000 1111を作用したマスク処理
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
		channelName = "(総目次/最初の画面)";
	}
	else if (channelNum >= 1 && channelNum < 20) {
		channelName = "(ニュース)";
	}
	else if (channelNum >= 20 && channelNum < 40) {
		channelName = "(一般情報番組)";
	}
	else if (channelNum >= 40 && channelNum < 80) {
		channelName = "(交通情報)";
	}
	else if (channelNum >= 80 && channelNum < 100) {
		channelName = "(番組情報)";
	}
	else if (channelNum >= 100 && channelNum < 150) {
		channelName = "(気象情報)";
	}
	else if (channelNum >= 150 && channelNum < 250) {
		channelName = "(その他)";
	}
	return;
}

//内容更新は不要？
//unsigned char  DecodeData::getUpdateNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
//}
unsigned char  DecodeData::getPageNum(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0011 1111を作用したマスク処理
	unsigned char page = (dataGroup[bp] & 0x3F);
	return page;
}

unsigned char  DecodeData::getMacroSign(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//4桁右ビットシフト(1111 0000)
	unsigned char macro = (dataGroup[bp] >> 4);
	return macro;
}

unsigned char  DecodeData::getPrefecture(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0011 1111を作用したマスク処理
	unsigned char prefec = (dataGroup[bp] & 0x3F);
	return prefec;
}

void  DecodeData::getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//3桁右ビットシフト(1111 1000)
	XMapNum = (dataGroup[bp] >> 3);

	//0000 0111をマスク
	XMeshNum = (dataGroup[bp] & 0x07);

	return;
}

void  DecodeData::getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//3桁右ビットシフト(1111 1000)
	YMapNum = (dataGroup[bp] >> 3);

	//0000 0111をマスク
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
	//0000 1000をマスク
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
	//1100 0000をマスク
	unsigned char linkLayerNum = ((dataGroup[bp] & 0xC0) >> 6);
	switch (linkLayerNum) {
	case 1:
		return "広域";
	case 2:
		return "中域";
	case 3:
		return "狭域";
	};
}

unsigned char DecodeData::getUnitType(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	unsigned char unitTypeNum = dataGroup[bp];
	return unitTypeNum;
}

unsigned char DecodeData::getDUL(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//1000 0000をマスク
	if ((dataGroup[bp] & 0x80) == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

unsigned int  DecodeData::getUnitSize(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0111 1111をマスク
	unsigned int unitSize = (dataGroup[bp] & 0x7F) * 16 * 16;
	bp++;
	unitSize += dataGroup[bp];

	return unitSize;
};

unsigned char  DecodeData::getSegmentType(std::string& segmentTypeName, std::vector<unsigned char>& dataGroup, uint32_t& bp, struct SelfSegmentParameter* selfSegmentParameter, struct OtherSegmentParameter* otherSegmentParameter) {
	//1111 0000をマスク
	unsigned char segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
	//自局用の場合
	if (segmentTypeNum != 0x0E) {
		segmentTypeName = selfSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
	//他局用の場合
	else {
		bp++;
		segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
		segmentTypeName = otherSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
}

unsigned char  DecodeData::getSegmentSize(std::vector<unsigned char>& dataGroup, uint32_t& bp) {
	//0000 1111をマスク
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

//SIのtest
//unsigned char DecodeData::testSI();

//デコードクラスのmain処理
void  DecodeData::callDecodeData(unsigned char(&dataPacket)[22])
{
	
	setPrefix(dataPacket, &prefixUnion);


	packetFlag = keep.getPacketFlag(&prefixUnion);

	//パケット番号をカウント
	packetCount++;
	printf("packetCount: %04d\n", packetCount);

	unsigned char ret = 0;

	//指定の位置にパケットデータが入っていない場合
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

		//KeepDataBlockクラスの指定の位置にデータブロックを格納する
		ret = keep.addDataBlock(&prefixUnion, dataBlock);
	}

	//データブロックが揃うまでループ
	if (ret != 1) {
		return;
	}

	//階層４のデータグループ
	std::vector<unsigned char> dataGroup;
	dataGroup = keep.getDataGroup(&prefixUnion);

	//使用しないメモリを解放
	//keep.clearDataGroup();

	//参照するバイト位置(BytePlace)を記憶
	uint32_t bp = 0;

	unsigned char endLoopFlag = 0;
	while (1) {
		while (1) {

			//セグメントデータの場合
			if (((prefixUnion.prefix >> 24) & 0x0f) == 0x0D) {
				//(n)セグメント識別取得
				segmentTypeNum = getSegmentType(segmentTypeName, dataGroup, bp, selfSegmentParameter, otherSegmentParameter);
				data.setData(18, 0, 0, segmentTypeName);

				//セグメントサイズ取得
				segmentSize = getSegmentSize(dataGroup, bp);
				//パケット番号取得
				data.setData(19, 0, packetCount, 0);
				
				//セグメントデータ取得
				bp++;
				getSegmentData(segmentData, dataGroup, bp);

				return;
			}

			//BD2 DGL取得
			bp++;
			dataGroupLink = getDataGroupLink(dataGroup, bp);

			//DB(n), +2　グループ構成取得
			bp += 2;
			dataGroupType = getDataGroupType(dataGroup, bp);

			//DB(n+1)　パラメータ取得
			bp++;
			//RS(データヘッダ)の場合
			if (dataGroupType == 0) {

				parameter = getParameter(parameterName, dataGroup, bp);
				//delete
				printf("parameter: %x\n", parameter);
				data.setData(2, 0, 0, 0);
				data.setData(3, 0, 0, parameterName);

				
				std::cout << parameterName << std::endl;

				//DB(n+2)　番組番号取得
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

				//DB(n+3) 内容更新、ページ総数/ページ番号取得
				bp++;
				if ((parameter >= 0 && parameter <= 4) || (parameter >= 8 && parameter <= 11)) {
					pageNum = getPageNum(dataGroup, bp);
					data.setData(6, pageNum, 0, 0);
				}



				//番組共通マクロ解析
				//DB(n+4) 番組共通マクロ集合取得
				
				if (parameter == 4 || parameter == 5) {
					bp++;
					macroSign = getMacroSign(dataGroup, bp);

					//マクロ集合符号が1もしくは2バイトの分岐
					if (macroSign == 2) {
						bp += 3;
					}
					if (macroSign == 3) {
						bp += 2;
					}

					//構成Aはここで終了
					if (parameter == 4) {
						break;
					}

					//(n+6/7) 都道府県取得
					prefectureNum = getPrefecture(dataGroup, bp);
					prefectureName = prefectureList[prefectureNum].prefectureName;

					//番組共通マクロはここで終了
					bp += 7;
					break;
				}

				//番組データヘッダの場合
				if (parameter == 0 || parameter == 1) {
					bp += 3;
				}
				//ページデータヘッダの場合
				if (parameter == 2 || parameter == 3) {
					bp +=4;
				}
				//構成Aはここで終了
				if (parameter == 0 || parameter == 2) {
					break;
				}

				//都道府県取得
				prefectureNum = getPrefecture(dataGroup, bp);
				prefectureName = prefectureList[prefectureNum].prefectureName;
				data.setData(7, prefectureNum, 0, 0);
				data.setData(8, 0, 0, prefectureName);

				//(n += 2)X座標　地図番号、二次メッシュ取得
				bp += 2;
				getXMapAddress(XMapNum, XMeshNum, dataGroup, bp);
				data.setData(9, XMapNum, 0, 0);

				//(n += 1)Y座標　地図番号、二次メッシュ取得
				bp++;
				getYMapAddress(YMapNum, YMeshNum, dataGroup, bp);
				data.setData(10, YMapNum, 0, 0);

				//MeshID、二次メッシュ取得
				meshID = getMeshID(XMapNum, YMapNum);
				data.setData(11, 0, meshID, 0);

				secondMesh = getSecondMesh(XMapNum, YMapNum, XMeshNum, YMeshNum);
				data.setData(12, 0, secondMesh, 0);

				//(n += 2)番組データヘッダはここで終了
				bp += 2;
				
				if (parameter == 1) {
					break;
				}
				//情報提供時刻取得
				timeFlag = getTime(time, dataGroup, bp);
				data.setData(13, 0, 0, time);

				//(n += 1)リンクレイヤ取得
				bp++;
				linkLayer = getLinkLayer(dataGroup, bp);
				data.setData(14, 0, 0, linkLayer);

				//(n += 2)ページデータヘッダ終了
				bp += 2;
				break;
			}
		}

		//長さ調整NULLをスキップ
		while (dataGroup[bp] == 0x00) {
			bp++;
		}

		if (dataGroup[bp] == 0x03 || dataGroup[bp] == 0x04 || dataGroup[bp] == 0x17) {
			return;
		}

		while (1) {

			//US(ユニットデータ)の場合
			dataGroupType = getDataGroupType(dataGroup, bp);
			if (dataGroupType == 1) {
				//(n += 1)データユニットタイプ取得
				
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

				//(n += 1)DUL取得
				bp += 1;
				DUL = getDUL(dataGroup, bp);

				//ユニットサイズ取得
				unitSize = getUnitSize(dataGroup, bp);
				data.setData(17, 0, unitSize, 0);

				//ユニットサイズ分スキップ
				bp += (unitSize + 1);
				if (dataGroup[bp] != 0x1F) {
					break;
				}
			}

		}
		//長さ調整NULLをスキップ
		while (dataGroup[bp] == 0x00) {
			bp++;
		}

		if (dataGroup[bp] == 0x03 || dataGroup[bp] == 0x04 || dataGroup[bp] == 0x17) {
			return;
		}
	}
}

