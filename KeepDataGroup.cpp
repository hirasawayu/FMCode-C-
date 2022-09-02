#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"
#include "DecodeData.h"

using std::vector;


vector<DataGroupCheck> dataGroupCheck(1, {0, 0});

//{*SI{*dataGroup{*PacketNum{ dataBlock, occupationFlag
	//vector<vector<要素の型>> 変数名(要素数1, vector<要素の型>(要素数2, 初期値));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(14, vector<vector<vector<unsigned char>>>(1, vector<vector<unsigned char>>(1, vector<unsigned char>(22))));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//データグループ番号を定義
	//付加情報以外
	//delete
	printf("prefix: %08x\n", prefixUnion->prefix);
	if (((prefixUnion->prefix >> 24) & 0x0F) != 0x0D) {
		SINum = prefixUnion->prefixData.SINum;
		packetNum = prefixUnion->prefixData.dataPacketNum;
		dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
		//delete
		printf("notD\n");
	}
	//付加情報
	else if (((prefixUnion->prefix >> 8) & 0x0F) == 0x0D) {
		SINum = prefixUnion->prefixDataD.SINum;
		packetNum = prefixUnion->prefixDataD.dataPacketNum;
		dataGroupNum = prefixUnion->prefixDataD.dataGroupNum;
		//delete
		printf("D\n");
	}

	printf("%x\n", packetNum);
	printf("Page: %x\n", prefixUnion->prefixData.PrePageNum);
	printf("Channel: %x\n", prefixUnion->prefixData.PreChannelNum);
	printf("SI: %x\n", SINum);
	printf("dataGroupNum: %x\n", dataGroupNum);
	printf("%x\n", prefixUnion->prefixData.decodeSign);
	printf("%x\n", prefixUnion->prefixData.endSign);
	printf("%x\n", prefixUnion->prefixData.updateSign);
	printf("nonused %x\n", prefixUnion->prefixDataD.non_used);

	//要素内にパケットデータが入っているかのフラグを返す
	//checkedGroupNumの配列にデータグループ番号の要素があるかを判定する
	unsigned char groupPointer = 0;
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
			groupPointer = dataGroupCheck[i].num;
		}
		else if (dataGroupCheck[i].checkedGroupNum != dataGroupNum) {
			return 0;
		}
	}

	if (packetNum >= keepDataGroup[SINum][groupPointer].size()) {
		return 0;
	}
	return keepDataGroup[SINum][dataGroupCheck[groupPointer].num][packetNum][20];
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {
	
	//データグループ番号を定義
	//付加情報以外
	if (((prefixUnion->prefix >> 24) & 0x0f) != 0x0D) {
		SINum = prefixUnion->prefixData.SINum;
		packetNum = prefixUnion->prefixData.dataPacketNum;
		dataGroupNum = SINum * 256 * 256 + prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
	}
	//付加情報
	else if (((prefixUnion->prefix >> 24) & 0x0f) == 0x0D) {
		SINum = prefixUnion->prefixDataD.SINum;
		packetNum = prefixUnion->prefixDataD.dataPacketNum;
		dataGroupNum = prefixUnion->prefixDataD.dataGroupNum;
	}
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

		keepDataGroup[SINum].push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		pushNum = dataGroupCheck.size();
		dataGroupCheck.push_back({ pushNum, dataGroupNum });
	}

	//mark
	// keepDataGroup[22] = {18byte(DataBlock用), 2byte(SI=D用), 1byte(DataBlockが入っているかのフラグ), 1byte(最後のDetaBlockがあるかのフラグ)}
	if (packetNum >= keepDataGroup[SINum][pushNum].size()) {
		for (unsigned char i = keepDataGroup[SINum][pushNum].size(); i <= packetNum; i++) {

			keepDataGroup[SINum][pushNum].push_back(vector<unsigned char>(22));
			//パケット番号以外の新しく追加した位置にはデータ、フラグ共に0を入れる
			if (i != packetNum) {
				keepDataGroup[SINum][pushNum][i][20] = { 0 };
			}
			//パケット番号の位置にはデータブロックを入れ、フラグを1にする
			else if (i == packetNum) {
				//[0]~[19]はデータブロックを代入
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][pushNum][i][j] = dataBlock[j];
					//delete
					printf("%02x ", keepDataGroup[SINum][pushNum][i][j]);
				}
				//delete
				printf("\n");
				printf("i: %d\n", i);
				//[20]にはパケットデータが入っているかを判断するフラグを代入
				keepDataGroup[SINum][pushNum][i][20] = { 1 };

				//データグループ最後のデータブロックが来たことを示す
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[SINum][pushNum][i][21] = {1};
					packetSize = packetNum;
				};


			};
		};
		
		//delete
		printf("lastPacket: %d\n", lastPacketFlag);
		printf("lastNum: %d\n", keepDataGroup[SINum][pushNum][packetNum][21] == 1);

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
			else {
				lastPacketFlag = 1;
			}
		}
 	
	}
	return 0;
}

//caution DataBlockの渡し方を検討、ポインタを用いるか
std::vector<unsigned char> KeepDataGroup::getDataGroup(union PrefixUnion* prefixUnion) {
	
	packetSize = keepDataGroup[SINum][pushNum].size();
	//delete
	printf("packetSize: %d\n", packetSize);
	std::vector <unsigned char>dataGroup;
	if (SINum != 0x0D) {
		for (char j = 0; j < packetSize; j++) {
			for (char i = 0; i < 18; i++) {
				dataGroup.push_back(keepDataGroup[SINum][pushNum][j][i]);
				printf("%02x ", dataGroup[j * 18 + i]);
			}
		}
	}
	else {
		for (char j = 0; j < packetSize; j++) {
			for (char i = 0; i < 20; i++) {
				dataGroup.push_back(keepDataGroup[SINum][pushNum][j][i]);
			}
		}
	}
	return dataGroup;
};

void KeepDataGroup::clearDataGroup() {

};


//要素の追加は
//keepDataGroup.at(0).push_back(1)
//SI0にグループ番号１を追加

//要素数の変更（初期化）は
//keepDataGroup.resize(0, vector<unsigned char>(0))

//要素数取得は
//std::size Numsize = keepDataGroup.size();