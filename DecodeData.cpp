#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "DecodeData.h"
#include "KeepDataGroup.h"

//KeepDataGroupのオブジェクトを生成
KeepDataGroup keep;



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

unsigned char DecodeData::bitreverse(unsigned char v)
{
	v = (v & 0x55555555) << 1 | (v >> 1 & 0x55555555);
	v = (v & 0x33333333) << 2 | (v >> 2 & 0x33333333);
	v = (v & 0x0f0f0f0f) << 4 | (v >> 4 & 0x0f0f0f0f);
	v = (v & 0x00ff00ff) << 8 | (v >> 8 & 0x00ff00ff);
	v = (v & 0x0000ffff) << 16 | (v >> 16 & 0x0000ffff);
	return v;
}

void DecodeData::setPrefix(unsigned char *packetData) {

	//付加情報の場合
	if (packetData[1] == 0x0B) {

		//各配列のbitを逆順にする
		unsigned char p1 = bitreverse(packetData[0]);
		unsigned char p2 = packetData[1];

		//逆順にした値を入れる配列を用意
		unsigned char prefix[2];

		//用意した配列に値をコピー
		memset(prefix, p1, 1);
		memset(&prefix[1], p2, 1);

	}

	else {
		
		unsigned char p1 = bitreverse(packetData[0]);
		unsigned char p2 = packetData[1];
		unsigned char p3 = packetData[2];
		unsigned char p4 = packetData[3];

		unsigned char prefix[4];

		memset(prefix, p1, 1);
		memset(&prefix[1], p2, 1);
		memset(&prefix[2], p3, 1);
		memset(&prefix[3], p4, 1);

		//SI、グループ、パケット番号に対応する位置にデータパケット後ろ18bitを格納
		unsigned char* pdataBlock = dataBlock;
		pdataBlock = &packetData[4];
	}
}

void DecodeData::createDataGroup(std::vector<unsigned char>& dataGroup) {

	keep.getDataGroup(prefixData);

	for (int i = 0; i > packetNum; i++) {

		std::vector<unsigned char> dataGroup =




	}

}

unsigned char DecodeData::getDataGroupLink(std::vector<unsigned char> &dataGroup, unsigned char &bp) {
	//1000 0000を作用したマスク処理
	if ((dataGroup[bp] & 0x80) == 0x80) {
		return 1;
	}
	else {
		return 0;
	}
}


unsigned char  DecodeData::getDataGroupType(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	
	if (dataGroup[bp] == 0x1E) {
		return 0;
	}
	else if (dataGroup[bp] == 0x1F) {
		return 1;
	}
}

unsigned char  DecodeData::getParameter(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0000 1111を作用したマスク処理
	unsigned char para = (dataGroup[bp] & 0x0F);
	return para;
}

void  DecodeData::getChannelInfo(unsigned char &channelNum, std::string &channelName, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	channelNum == dataGroup[bp];
	if (channelNum == 0) {
		std::string channelName = "(総目次/最初の画面)";
	}
	else if (channelNum >= 1 && channelNum < 20) {
		std::string channelName = "(ニュース)";
	}
	else if (channelNum >= 20 && channelNum < 40) {
		std::string channelName = "(一般情報番組)";
	}
	else if (channelNum >= 40 && channelNum < 80) {
		std::string channelName = "(交通情報)";
	}
	else if (channelNum >= 80 && channelNum < 100) {
		std::string channelName = "(番組情報)";
	}
	else if (channelNum >= 100 && channelNum < 150) {
		std::string channelName = "(気象情報)";
	}
	else if (channelNum >= 150 && channelNum < 250) {
		std::string channelName = "(その他)";
	}
	return;
}

//内容更新は不要？
//unsigned char  DecodeData::getUpdateNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
//}
unsigned char  DecodeData::getPageNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0011 1111を作用したマスク処理
	unsigned char page = (dataGroup[bp] & 0x3F);
	return page;
}

unsigned char  DecodeData::getMacroSign(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//4桁右ビットシフト(1111 0000)
	unsigned char macro = (dataGroup[bp] >> 4);
	return macro;
}

unsigned char  DecodeData::getPrefecture(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0011 1111を作用したマスク処理
	unsigned char prefec = (dataGroup[bp] & 0x3F);
	return prefec;
}

void  DecodeData::getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//3桁右ビットシフト(1111 1000)
	XMapNum = (dataGroup[bp] >> 3);

	//0000 0111をマスク
	XMeshNum = (dataGroup[bp] & 0x07);

	return;
}

void  DecodeData::getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
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

unsigned char  DecodeData::getTime(std::string &time, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0000 1000をマスク
	unsigned char timeFlag = (dataGroup[bp] & 0x08);
	if (timeFlag == 1) {
		unsigned char hour = ((dataGroup[bp] & 0x07) << 3) + ((dataGroup[bp + 1] & 0xC0) >> 6);
		unsigned char minute = (dataGroup[bp + 1] & 0x3F);

		time = hour + ":" + minute;
	}
	return timeFlag;
}

std::string DecodeData::getLinkLayer(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
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

unsigned char DecodeData::getUnitType(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	unsigned char unitTypeNum = dataGroup[bp];
	return unitTypeNum;
}

unsigned char DecodeData::getDUL(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//1000 0000をマスク
	if ((dataGroup[bp] & 0x80) == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

unsigned int  DecodeData::getUnitSize(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0111 1111をマスク
	unsigned int unitSize = ((dataGroup[bp] & 0x7F) * 16 * 16 + (dataGroup[bp + 1]));
	return unitSize;
}

unsigned char  DecodeData::getSegmentType(std::string& segmentTypeName, std::vector<unsigned char>& dataGroup, unsigned char& bp, struct SelfSegmentParameter* selfSegmentParameter, struct OtherSegmentParameter* otherSegmentParameter) {
	//1111 0000をマスク
	unsigned char segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
	//自局用の場合
	if (segmentTypeNum != 0x0E) {
		segmentTypeName = selfSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
	//他局用の場合
	else if (segmentTypeNum == 0x0E) {
		bp++;
		segmentTypeNum = ((dataGroup[bp] & 0xF0) >> 4);
		segmentTypeName = otherSegmentParameter->segmentTypeName[segmentTypeNum];
		return segmentTypeNum;
	}
}

unsigned char  DecodeData::getSegmentSize() {

}



unsigned char  DecodeData::getEndGroupFlag() {

}


//デコードクラスのmain処理
void  DecodeData::callDecodeData(union PrefixData &prefixData, struct PrefectureList &prefectureList, struct UnitParameter unitParameter)
{
	while (1) {

		unsigned char dataPacket[22];

		setPrefix(&dataPacket[22]);

		//caution SINum等が共用体から取得できているかを確認する


		packetFlag = keep.getPacketFlag(prefixData);

		//指定の位置にパケットデータが入っていない場合
		if (packetFlag = 0 || NULL) {


			//KeepDataBlockクラスの指定の位置にデータブロックを格納する
			unsigned char ret = keep.addDataBlock(prefixData.SINum, prefixData.dataGroupNum,prefixData.dataPacketNum, dataBlock);

			//データブロックが揃うまでループ
			if (ret == 1) {
				break;

			}
		}
	}

	std::vector<unsigned char> dataGroup;
	//caution 作成したデータグループを入れる配列をどうやってもってくるか、データグループ作成
	createDataGroup(dataGroup);

	//参照するバイト位置(BytePlace)を記憶
	unsigned char bp = 0;
	//while

	while (1) {

		//セグメントデータの場合
		if (prefixData.SINum == 0x0D) {
			//(n)セグメント識別取得
			segmentTypeNum = getSegmentType(segmentTypeName, dataGroup, bp, selfSegmentParameter, otherSegmentParameter);
			
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
		if (dataGroupType = 0) {

			parameter = getParameter(dataGroup, bp);

			//DB(n+2)　番組番号取得
			bp++;
			if (parameter == 0 || 1 || 2 || 3 || 4 || 8 || 9 || 10 || 11) {
				getChannelInfo(channelNum, channelName, dataGroup, bp);
			}
			else if (parameter = 6) {
				break;
			}

			//DB(n+3) 内容更新、ページ総数/ページ番号取得
			bp++;
			if (parameter == 0 || 1 || 2 || 3 || 4 || 8 || 9 || 10 || 11) {
				pageNum = getPageNum(dataGroup, bp);
			}

			//番組共通マクロ解析
			//DB(n+4) 番組共通マクロ集合取得
			bp++;
			if (parameter == 4 || 5) {
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
				prefectureName = prefectureList.prefectureName[prefectureNum];

				//番組共通マクロはここで終了
				bp += 7;
				break;
			}

			//番組データヘッダの場合
			if (parameter == 0 || 1) {
				bp += 3;
			}
			//ページデータヘッダの場合
			if (parameter == 2 || 3) {
				bp = +4;
			}
			//構成Aはここで終了
			if (parameter == 0 || 2) {
				break;
			}

			//都道府県取得
			prefectureNum = getPrefecture(dataGroup, bp);
			prefectureName = prefectureList.prefectureName[prefectureNum];

			//(n += 2)X座標　地図番号、二次メッシュ取得
			bp += 2;
			getXMapAddress(XMapNum, XMeshNum, dataGroup, bp);

			//(n += 1)Y座標　地図番号、二次メッシュ取得
			bp++;
			getYMapAddress(YMapNum, YMeshNum, dataGroup, bp);

			//MeshID、二次メッシュ取得
			meshID = getMeshID(XMapNum, YMapNum);

			secondMesh = getSecondMesh(XMapNum, YMapNum, XMeshNum, YMeshNum);

			//(n += 2)番組データヘッダはここで終了
			bp += 2;
			if (parameter == 0) {
				break;
			}
			//情報提供時刻取得
			timeFlag = getTime(time, dataGroup, bp);

			//(n += 1)リンクレイヤ取得
			linkLayer == getLinkLayer(dataGroup, bp);

			//(n += 2)ページデータヘッダ終了
			bp += 2;
			break;
		}

		//US(ユニットデータ)の場合
		else if (dataGroupType == 1) {
			//(n += 1)データユニットタイプ取得
			bp += 1;
			unitTypeNum = getUnitType(dataGroup, bp);
			unitTypeName = unitParameter.unitTypeName[unitTypeNum];

			//(n += 1)DUL取得
			bp += 1;
			DUL = getDUL(dataGroup, bp);

			//ユニットサイズ取得
			unitSize = getUnitSize(dataGroup, bp);

			//ユニットサイズ分スキップ
			bp += (2 + unitSize);
		}
	}
}

