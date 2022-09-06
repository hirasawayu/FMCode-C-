#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"
#include "DecodeData.h"
#include "DataBase.h"

using std::vector;


vector<DataGroupCheck> dataGroupCheck(0, {0, 0});

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag
	//vector<vector<要素の型>> 変数名(要素数1, vector<要素の型>(要素数2, 初期値));
vector<vector<vector<unsigned char>>>keepDataGroup(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22)));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//データグループ番号を定義
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	if (dataGroupCheck.size() == 0) {
		return 0;
	}
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (dataGroupCheck[i].actualGroupNum == ActualGroupNum) {
			OrderGroupNum = dataGroupCheck[i].orderGroupNum;
		}
		else {
			return 0;
		}
	}

	//パケット番号、セグメントフラグを取得
	unsigned char PacketNum;
	if (((prefixUnion->prefix >> 8) & 0x0F) == 0x0D) {
		PacketNum = prefixUnion->prefixDataD.dataPacketNum;
		segmentFlag = 1;
	}
	else {
		PacketNum = prefixUnion->prefixData.dataPacketNum;
		segmentFlag = 0;
	}

	//要素内にパケットデータが入っているかのフラグを返す
	if (PacketNum < keepDataGroup.at(OrderGroupNum).size()) {
		return keepDataGroup[OrderGroupNum][PacketNum][20];
	}
	else {
		return 0;
	}
	
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {
	
	
	//データグループ番号を定義
	//付加情報
	if (segmentFlag == 1) {
		ActualGroupNum = prefixUnion->prefixDataD.SINum * 16 + prefixUnion->prefixDataD.dataGroupNum;
	}
	else {
		ActualGroupNum = prefixUnion->prefixData.SINum * 256 * 256 + prefixUnion->prefixData.preChannelNum * 256 + prefixUnion->prefixData.prePageNum;
	}

	if (ActualGroupNum == 0x0F0101) {
		return 0;
	}

	//SI、データグループ、データパケット番号が無い場合、要素を追加する
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	OrderGroupNum = 0xFFFFFFFF;
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (ActualGroupNum == dataGroupCheck[i].actualGroupNum) {
			OrderGroupNum = dataGroupCheck[i].orderGroupNum;
		}
	}

	//無い場合は配列の要素に追加し、構造体dataGroupCheckに記録する
	if (OrderGroupNum == 0xFFFFFFFF) {

		//新規のデータブロックを入れる際のorderGroupNumの値
		OrderGroupNum = dataGroupCheck.size();

		keepDataGroup.push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		dataGroupCheck.push_back({ OrderGroupNum, ActualGroupNum });
	}

	//delete
	printf("orderNum: %02x\n", dataGroupCheck[OrderGroupNum].orderGroupNum);
	printf("actualNum: %02x\n", dataGroupCheck[OrderGroupNum].actualGroupNum);
	//パケット番号を取得
	unsigned char PacketNum;
	if (segmentFlag == 1) {
		PacketNum = prefixUnion->prefixDataD.dataPacketNum;
	}
	else {
		PacketNum = prefixUnion->prefixData.dataPacketNum;
	}

	// keepDataGroup[22] = {18byte(DataBlock用), 2byte(SI=D用), 1byte(DataBlockが入っているかのフラグ), 1byte(最後のDetaBlockがあるかのフラグ)}
	if (PacketNum >= keepDataGroup[OrderGroupNum].size()) {
		for (unsigned char i = keepDataGroup[OrderGroupNum].size(); i <= PacketNum; i++) {

			keepDataGroup[OrderGroupNum].push_back(vector<unsigned char>(22));
			//パケット番号以外の新しく追加した位置にはデータ、フラグ共に0を入れる
			if (i != PacketNum) {
				keepDataGroup[OrderGroupNum][i][20] = { 0 };
			}

			//パケット番号の位置にはデータブロックを入れ、フラグを1にする
			else if (i == PacketNum) {
				//[0]~[19]はデータブロックを代入
				for (char j = 0; j < 20; j++) {

					keepDataGroup[OrderGroupNum][i][j] = dataBlock[j];
					//delete
					printf("%02x ", keepDataGroup[OrderGroupNum][i][j]);
				}
				//delete
				printf("\n");
				//[20]にはパケットデータが入っているかを判断するフラグを代入
				keepDataGroup[OrderGroupNum][i][20] = 1;

				//データグループ最後のデータブロックが来たことを示す
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[OrderGroupNum][i][21] = 1;
					packetSize = PacketNum;
				};
			};
		};

		//最後のデータブロックがある、かつ全てのデータブロックが揃った場合
		if (keepDataGroup[OrderGroupNum][PacketNum][21] == 1 || lastPacketFlag == 1) {
			char occupationFlagCount = 0;

			for (unsigned char i = 0; i < PacketNum; i++) {
				if (keepDataGroup[OrderGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				}
			}

			if (occupationFlagCount == (packetSize)) {
				lastPacketFlag = 0;
				//全てのデータブロックが揃った場合は1を返す
				return 1;
			}
			else {
				lastPacketFlag = 1;
			}
		}
	}
	return 0;
}

//caution DataBlockの渡し方を検討、ポインタを用いるか
std::vector<unsigned char> KeepDataGroup::getDataGroup(union PrefixUnion* prefixUnion) {
	
	packetSize = keepDataGroup[OrderGroupNum].size();

	std::vector <unsigned char>dataGroup(0);
	if (segmentFlag == 0) {
		//warn
		for (int j = 0; j < packetSize; j++) {
			for (char i = 0; i < 18; i++) {
				dataGroup.push_back(keepDataGroup[OrderGroupNum][j][i]);
				printf("%02x ", dataGroup[j * 18 + i]);
			}
		}
	}
	else {
		for (int j = 0; j < packetSize; j++) {
			for (char i = 0; i < 20; i++) {
				dataGroup.push_back(keepDataGroup[OrderGroupNum][j][i]);
				printf("%02x ", dataGroup[j * 20 + i]);
			}
		}
	}
	printf("\n");
	return dataGroup;
};

void KeepDataGroup::clearDataGroup() {
	keepDataGroup[OrderGroupNum].erase(keepDataGroup[OrderGroupNum].begin(), keepDataGroup[OrderGroupNum].end());
	keepDataGroup[OrderGroupNum].shrink_to_fit();
};


//要素の追加は
//keepDataGroup.at(0).push_back(1)
//SI0にグループ番号１を追加

//要素数の変更（初期化）は
//keepDataGroup.resize(0, vector<unsigned char>(0))

//要素数取得は
//std::size Numsize = keepDataGroup.size();