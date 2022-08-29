#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"

int KeepDataGroup::getPacketFlag(struct prefixData &prefix) {
	
	//要素内にパケットデータが入っているかのフラグを返す
	return keepDataGroup.at(prefix.SINum).at(dataGroupNum).at(dataPacketNum).at(20);
}

int KeepDataGroup::addDataBlock(unsigned char SINum, unsigned char dataGroupNum, unsigned char dataPacketNum, unsigned char dataBlock[20], struct prefixData &prefix) {
	
	//SI、データグループ、データパケット番号が無い場合、要素を追加する
	// keepDataGroup[22] = {18byte(DataBlock用), 2byte(SI=D用), 1byte(DataBlockが入っているかのフラグ), 1byte(最後のDetaBlockがあるかのフラグ)}
	//注意：各配列(4階層目は除く)の[0]番目要素には何も入れない（0が入る
	if (dataPacketNum > keepDataGroup.at(SINum).at(dataGroupNum).at(dataPacketNum).size()) {
		for (char i = keepDataGroup.at(SINum).at(dataGroupNum).at(dataPacketNum).size() + 1; i <= SINum; i++) {
			
			keepDataGroup.at(SINum).at(dataGroupNum).at(i).push_back( i );
				//パケット番号以外の新しく追加した位置にはデータ、フラグ共に0を入れる
			if (i != dataPacketNum) {
				keepDataGroup[SINum][dataGroupNum][i][20] = { 0 };
			}
			//パケット番号の位置にはデータブロックを入れ、フラグを1にする
			else if (i == dataPacketNum) {
				//[0]~[19]はデータブロックを代入
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][dataGroupNum][i][j] = dataBlock[j];
				}
				//[20]にはパケットデータが入っているかを判断するフラグを代入
				keepDataGroup[SINum][dataGroupNum][i][20] = { 1 };

				//データグループ最後のデータブロックが来たことを示す
				if (prefix.endSign == 1) {

					keepDataGroup[SINum][dataGroupNum][i][21] = { 1 };
				};


			};
		};

		//最後のデータブロックがある、かつ全てのデータブロックが揃った場合
		if (keepDataGroup[SINum][dataGroupNum][dataPacketNum][21] == 1) {
			char occupationFlagCount = 0;

			for (char i = 1; i <= dataPacketNum; i++) {
				if (keepDataGroup[SINum][dataGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == dataPacketNum) {
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
void KeepDataGroup::getDataGroup(unsigned char &SINum, unsigned char &dataGroupNum, unsigned char &lastPacketNum) {

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