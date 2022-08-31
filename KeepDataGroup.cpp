#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"
#include "DecodeData.h"


std::vector<DataGroupCheck> dataGroupCheck;


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
		}

	return keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupCheck[groupPointer].num).at(prefixUnion->prefixData.dataPacketNum).at(20);
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {

	//データグループ番号を定義
	unsigned int dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
	
	//SI、データグループ、データパケット番号が無い場合、要素を追加する
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	auto findGroupNum = std::find(dataGroupCheck.begin(), dataGroupCheck.end(), dataGroupNum);

	//無い場合は配列の要素に追加し、構造体dataGroupCheckに記録する
	if (findGroupNum == dataGroupCheck.end()) {
		
		keepDataGroup.at(prefixUnion->prefixData.SINum).push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		unsigned char pushNum = dataGroupCheck.size() + 1;
		dataGroupCheck.push_back({ pushNum, dataGroupNum });
	}
	
	// keepDataGroup[22] = {18byte(DataBlock用), 2byte(SI=D用), 1byte(DataBlockが入っているかのフラグ), 1byte(最後のDetaBlockがあるかのフラグ)}
	//注意：各配列(4階層目は除く)の[0]番目要素には何も入れない（0が入る
	if (prefixUnion->prefixData.dataPacketNum > keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(prefixUnion->prefixData.dataPacketNum).size()) {
		for (char i = keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(prefixUnion->prefixData.dataPacketNum).size() + 1; i <= prefixUnion->prefixData.SINum; i++) {
			
			keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(i).push_back( i );
				//パケット番号以外の新しく追加した位置にはデータ、フラグ共に0を入れる
			if (i != prefixUnion->prefixData.dataPacketNum) {
				keepDataGroup[prefixUnion->prefixData.SINum][dataGroupNum][i][20] = { 0 };
			}
			//パケット番号の位置にはデータブロックを入れ、フラグを1にする
			else if (i == prefixUnion->prefixData.dataPacketNum) {
				//[0]~[19]はデータブロックを代入
				for (char j = 0; j < 20; j++) {

					keepDataGroup[prefixUnion->prefixData.SINum][dataGroupNum][i][j] = dataBlock[j];
				}
				//[20]にはパケットデータが入っているかを判断するフラグを代入
				keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][20] = { 1 };

				//データグループ最後のデータブロックが来たことを示す
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][21] = { 1 };
				};


			};
		};

		//最後のデータブロックがある、かつ全てのデータブロックが揃った場合
		if (keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][prefixUnion->prefixData.dataPacketNum][21] == 1) {
			char occupationFlagCount = 0;

			for (char i = 1; i <= prefixUnion->prefixData.dataPacketNum; i++) {
				if (keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == prefixUnion->prefixData.dataPacketNum) {
				//全てのデータブロックが揃った場合は1を返す
				return 1;
			};
		}
		//揃っていない場合は0を返す
		else {
			return 0;
		};
	};
};

//caution DataBlockの渡し方を検討、ポインタを用いるか
void KeepDataGroup::getDataGroup(union PrefixUnion** prefixUnion) {

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