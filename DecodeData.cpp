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

unsigned char DecodeData::bitreverse(unsigned char v)
{
	v = (v & 0x55555555) << 1 | (v >> 1 & 0x55555555);
	v = (v & 0x33333333) << 2 | (v >> 2 & 0x33333333);
	v = (v & 0x0f0f0f0f) << 4 | (v >> 4 & 0x0f0f0f0f);
	v = (v & 0x00ff00ff) << 8 | (v >> 8 & 0x00ff00ff);
	v = (v & 0x0000ffff) << 16 | (v >> 16 & 0x0000ffff);
	return v;
}

void DecodeData::setPrefix(unsigned char packetData[22]) {

	//付加情報の場合
	if (packetData[1] == 'B') {

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

//caution unsigned charの連結
int DecodeData::getMeshID(unsigned char XMapNum, unsigned char YMapNum) {
	int meshID = ;
	return meshID;
}

int DecodeData::getSecondMesh(unsigned char XMapNum, unsigned char YMapNum, unsigned char XMeshNum, unsigned char YMeshNum) {
	int secondMesh = (YMapNum + 37) * 10000 + (XMapNum + 22) * 100 + YMeshNum * 10 + XMeshNum;
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
	}
}

unsigned char  DecodeData::getEndGroupFlag() {

}



unsigned char  DecodeData::getDataUnitSize() {

}

unsigned char  DecodeData::getDataUnitType() {


}

unsigned char  DecodeData::getSegmentSize() {

}

unsigned char  DecodeData::getSegmentType() {

}





//デコードクラスのmain処理
void  DecodeData::callDecodeData(struct PrefixData &prefixData, struct PrefectureList &prefectureList)
{
	while (1) {

		unsigned char dataPacket[22];

		setPrefix(dataPacket);

		//caution SINum等が共用体から取得できているかを確認する


		packetFlag = keep.getPacketFlag(prefixData);

		//指定の位置にパケットデータが入っていない場合
		if (packetFlag = 0 || NULL) {


			//KeepDataBlockクラスの指定の位置にデータブロックを格納する
			ret = keep.addDataBlock(SINum, dataGroupNum, dataPacketNum, dataBlock, prefix);

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

	while (1) {

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

		}

	}
}

