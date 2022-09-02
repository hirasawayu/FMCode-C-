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
	//vector<vector<�v�f�̌^>> �ϐ���(�v�f��1, vector<�v�f�̌^>(�v�f��2, �����l));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(14, vector<vector<vector<unsigned char>>>(1, vector<vector<unsigned char>>(1, vector<unsigned char>(22))));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//�f�[�^�O���[�v�ԍ����`
	//�t�����ȊO
	//delete
	printf("prefix: %08x\n", prefixUnion->prefix);
	if (((prefixUnion->prefix >> 24) & 0x0F) != 0x0D) {
		SINum = prefixUnion->prefixData.SINum;
		packetNum = prefixUnion->prefixData.dataPacketNum;
		dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
		//delete
		printf("notD\n");
	}
	//�t�����
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

	//�v�f���Ƀp�P�b�g�f�[�^�������Ă��邩�̃t���O��Ԃ�
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
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
	
	//�f�[�^�O���[�v�ԍ����`
	//�t�����ȊO
	if (((prefixUnion->prefix >> 24) & 0x0f) != 0x0D) {
		SINum = prefixUnion->prefixData.SINum;
		packetNum = prefixUnion->prefixData.dataPacketNum;
		dataGroupNum = SINum * 256 * 256 + prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
	}
	//�t�����
	else if (((prefixUnion->prefix >> 24) & 0x0f) == 0x0D) {
		SINum = prefixUnion->prefixDataD.SINum;
		packetNum = prefixUnion->prefixDataD.dataPacketNum;
		dataGroupNum = prefixUnion->prefixDataD.dataGroupNum;
	}
	//�V�K�̃f�[�^�u���b�N������ۂ�dataGroupCheck.num�̒l
	pushNum = dataGroupCheck.size() + 1;

	//SI�A�f�[�^�O���[�v�A�f�[�^�p�P�b�g�ԍ��������ꍇ�A�v�f��ǉ�����
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
	pushNum = NULL;
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
			pushNum = dataGroupCheck[i].num;
		}
	}

	//�����ꍇ�͔z��̗v�f�ɒǉ����A�\����dataGroupCheck�ɋL�^����
	if (pushNum == NULL) {

		keepDataGroup[SINum].push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		pushNum = dataGroupCheck.size();
		dataGroupCheck.push_back({ pushNum, dataGroupNum });
	}

	//mark
	// keepDataGroup[22] = {18byte(DataBlock�p), 2byte(SI=D�p), 1byte(DataBlock�������Ă��邩�̃t���O), 1byte(�Ō��DetaBlock�����邩�̃t���O)}
	if (packetNum >= keepDataGroup[SINum][pushNum].size()) {
		for (unsigned char i = keepDataGroup[SINum][pushNum].size(); i <= packetNum; i++) {

			keepDataGroup[SINum][pushNum].push_back(vector<unsigned char>(22));
			//�p�P�b�g�ԍ��ȊO�̐V�����ǉ������ʒu�ɂ̓f�[�^�A�t���O����0������
			if (i != packetNum) {
				keepDataGroup[SINum][pushNum][i][20] = { 0 };
			}
			//�p�P�b�g�ԍ��̈ʒu�ɂ̓f�[�^�u���b�N�����A�t���O��1�ɂ���
			else if (i == packetNum) {
				//[0]~[19]�̓f�[�^�u���b�N����
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][pushNum][i][j] = dataBlock[j];
					//delete
					printf("%02x ", keepDataGroup[SINum][pushNum][i][j]);
				}
				//delete
				printf("\n");
				printf("i: %d\n", i);
				//[20]�ɂ̓p�P�b�g�f�[�^�������Ă��邩�𔻒f����t���O����
				keepDataGroup[SINum][pushNum][i][20] = { 1 };

				//�f�[�^�O���[�v�Ō�̃f�[�^�u���b�N���������Ƃ�����
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[SINum][pushNum][i][21] = {1};
					packetSize = packetNum;
				};


			};
		};
		
		//delete
		printf("lastPacket: %d\n", lastPacketFlag);
		printf("lastNum: %d\n", keepDataGroup[SINum][pushNum][packetNum][21] == 1);

		//�Ō�̃f�[�^�u���b�N������A���S�Ẵf�[�^�u���b�N���������ꍇ
		if (keepDataGroup[SINum][pushNum][packetNum][21] == 1 || lastPacketFlag == 1) {
			char occupationFlagCount = 0;

			for (unsigned char i = 1; i <= packetNum; i++) {
				if (keepDataGroup[SINum][pushNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == packetSize) {
				lastPacketFlag = 0;
				//�S�Ẵf�[�^�u���b�N���������ꍇ��1��Ԃ�
				return 1;
			}
			else {
				lastPacketFlag = 1;
			}
		}
 	
	}
	return 0;
}

//caution DataBlock�̓n�����������A�|�C���^��p���邩
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


//�v�f�̒ǉ���
//keepDataGroup.at(0).push_back(1)
//SI0�ɃO���[�v�ԍ��P��ǉ�

//�v�f���̕ύX�i�������j��
//keepDataGroup.resize(0, vector<unsigned char>(0))

//�v�f���擾��
//std::size Numsize = keepDataGroup.size();