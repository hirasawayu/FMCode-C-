#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"
#include "DecodeData.h"

using std::vector;


std::vector<DataGroupCheck> dataGroupCheck;

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag
	//vector<vector<要素の型>> 変数名(要素数1, vector<要素の型>(要素数2, 初期値));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(13, vector<vector<vector<unsigned char>>>(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22))));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//データグループ番号を定義
	unsigned int dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;

	//要素内にパケットデータが入っているかのフラグを返す
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	unsigned char groupPointer;
		for (char i = 0; i < dataGroupCheck.size(); i++) {
			if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
				groupPointer = dataGroupCheck[i].num;
			}
			else if (dataGroupCheck[i].checkedGroupNum != dataGroupNum) {
				return 0;
			}
		}

	return keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupCheck[groupPointer].num).at(prefixUnion->prefixData.dataPacketNum).at(20);
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {

	SINum = prefixUnion->prefixData.SINum;
	packetNum = prefixUnion->prefixData.dataPacketNum;

	//データグループ番号を定義
	dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
	//新規のデータブロックを入れる際のdataGroupCheck.numの値
	pushNum = dataGroupCheck.size() + 1;

	//SI、データグループ、データパケット番号が無い場合、要素を追加する
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	pushNum = NULL;
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
			pushNum = dataGroupCheck[i].num;
		}
	}

	//無い場合は配列の要素に追加し、構造体dataGroupCheckに記録する
	if (pushNum == NULL) {

		keepDataGroup.at(SINum).push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		pushNum = dataGroupCheck.size();
		dataGroupCheck.push_back({ pushNum, dataGroupNum });
	}

	else if (pushNum != NULL) {
		for (char i = 0; i < dataGroupCheck.size(); i++) {
			if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
				pushNum = dataGroupCheck[i].num;
			}
		}
	}

	// keepDataGroup[22] = {18byte(DataBlock用), 2byte(SI=D用), 1byte(DataBlockが入っているかのフラグ), 1byte(最後のDetaBlockがあるかのフラグ)}
	//注意：各配列(4階層目は除く)の[0]番目要素には何も入れない（0が入る
	if (packetNum > keepDataGroup.at(SINum).at(pushNum).at(packetNum).size()) {
		for (unsigned char i = keepDataGroup.at(SINum).at(pushNum).at(packetNum).size() + 1; i <= SINum; i++) {

			keepDataGroup.at(SINum).at(pushNum).at(i).push_back(i);
			//パケット番号以外の新しく追加した位置にはデータ、フラグ共に0を入れる
			if (i != packetNum) {
				keepDataGroup[SINum][pushNum][i][20] = { 0 };
			}
			//パケット番号の位置にはデータブロックを入れ、フラグを1にする
			else if (i == packetNum) {
				//[0]~[19]はデータブロックを代入
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][pushNum][i][j] = dataBlock[j];
				}
				//[20]にはパケットデータが入っているかを判断するフラグを代入
				keepDataGroup[SINum][pushNum][i][20] = { 1 };

				//データグループ最後のデータブロックが来たことを示す
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup.at(SINum).at(pushNum).at(i).at(21) = {1};
					packetSize = packetNum;
				};


			};
		};

		//最後のデータブロックがある、かつ全てのデータブロックが揃った場合
		if (keepDataGroup[SINum][pushNum][packetNum][21] == 1 || lastPacketFlag == 1) {
			char occupationFlagCount = 0;

			for (unsigned char i = 1; i <= packetNum; i++) {
				if (keepDataGroup[SINum][pushNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == packetSize) {
				lastPacketFlag = 0;
				//全てのデータブロックが揃った場合は1を返す
				return 1;
			}
		}
		//揃っていない場合は0を返す
		else {
			lastPacketFlag = 1;
			return 0;
		}
	}
}
//caution DataBlockの渡し方を検討、ポインタを用いるか
void KeepDataGroup::getDataGroup(vector<unsigned char>& dataGroup, union PrefixUnion* prefixUnion) {
	
	for (char j = 0; j < packetSize; j++) {
		for (char i = 0; i < 18; i++) {
			dataGroup.push_back(keepDataGroup[SINum][pushNum][j][i]);
		}
	}
	return;
};

void KeepDataGroup::clearDataGroup() {

};


//各要素のアクセスは
//keepDataGroup.at(0).at(1).at(2).at(3)

//要素の追加は
//keepDataGroup.at(0).push_back(1)
//SI0にグループ番号１を追加

//要素数の変更（初期化）は
//keepDataGroup.resize(0, vector<unsigned char>(0))

//要素数取得は
//std::size Numsize = keepDataGroup.size();