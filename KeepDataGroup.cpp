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
	//vector<vector<�v�f�̌^>> �ϐ���(�v�f��1, vector<�v�f�̌^>(�v�f��2, �����l));
vector<vector<vector<unsigned char>>>keepDataGroup(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22)));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//�f�[�^�O���[�v�ԍ����`
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
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

	//�p�P�b�g�ԍ��A�Z�O�����g�t���O���擾
	unsigned char PacketNum;
	if (((prefixUnion->prefix >> 8) & 0x0F) == 0x0D) {
		PacketNum = prefixUnion->prefixDataD.dataPacketNum;
		segmentFlag = 1;
	}
	else {
		PacketNum = prefixUnion->prefixData.dataPacketNum;
		segmentFlag = 0;
	}

	//�v�f���Ƀp�P�b�g�f�[�^�������Ă��邩�̃t���O��Ԃ�
	if (PacketNum < keepDataGroup.at(OrderGroupNum).size()) {
		return keepDataGroup[OrderGroupNum][PacketNum][20];
	}
	else {
		return 0;
	}
	
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {
	
	
	//�f�[�^�O���[�v�ԍ����`
	//�t�����
	if (segmentFlag == 1) {
		ActualGroupNum = prefixUnion->prefixDataD.SINum * 16 + prefixUnion->prefixDataD.dataGroupNum;
	}
	else {
		ActualGroupNum = prefixUnion->prefixData.SINum * 256 * 256 + prefixUnion->prefixData.preChannelNum * 256 + prefixUnion->prefixData.prePageNum;
	}

	if (ActualGroupNum == 0x0F0101) {
		return 0;
	}

	//SI�A�f�[�^�O���[�v�A�f�[�^�p�P�b�g�ԍ��������ꍇ�A�v�f��ǉ�����
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
	OrderGroupNum = 0xFFFFFFFF;
	for (char i = 0; i < dataGroupCheck.size(); i++) {
		if (ActualGroupNum == dataGroupCheck[i].actualGroupNum) {
			OrderGroupNum = dataGroupCheck[i].orderGroupNum;
		}
	}

	//�����ꍇ�͔z��̗v�f�ɒǉ����A�\����dataGroupCheck�ɋL�^����
	if (OrderGroupNum == 0xFFFFFFFF) {

		//�V�K�̃f�[�^�u���b�N������ۂ�orderGroupNum�̒l
		OrderGroupNum = dataGroupCheck.size();

		keepDataGroup.push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		dataGroupCheck.push_back({ OrderGroupNum, ActualGroupNum });
	}

	//delete
	printf("orderNum: %02x\n", dataGroupCheck[OrderGroupNum].orderGroupNum);
	printf("actualNum: %02x\n", dataGroupCheck[OrderGroupNum].actualGroupNum);
	//�p�P�b�g�ԍ����擾
	unsigned char PacketNum;
	if (segmentFlag == 1) {
		PacketNum = prefixUnion->prefixDataD.dataPacketNum;
	}
	else {
		PacketNum = prefixUnion->prefixData.dataPacketNum;
	}

	// keepDataGroup[22] = {18byte(DataBlock�p), 2byte(SI=D�p), 1byte(DataBlock�������Ă��邩�̃t���O), 1byte(�Ō��DetaBlock�����邩�̃t���O)}
	if (PacketNum >= keepDataGroup[OrderGroupNum].size()) {
		for (unsigned char i = keepDataGroup[OrderGroupNum].size(); i <= PacketNum; i++) {

			keepDataGroup[OrderGroupNum].push_back(vector<unsigned char>(22));
			//�p�P�b�g�ԍ��ȊO�̐V�����ǉ������ʒu�ɂ̓f�[�^�A�t���O����0������
			if (i != PacketNum) {
				keepDataGroup[OrderGroupNum][i][20] = { 0 };
			}

			//�p�P�b�g�ԍ��̈ʒu�ɂ̓f�[�^�u���b�N�����A�t���O��1�ɂ���
			else if (i == PacketNum) {
				//[0]~[19]�̓f�[�^�u���b�N����
				for (char j = 0; j < 20; j++) {

					keepDataGroup[OrderGroupNum][i][j] = dataBlock[j];
					//delete
					printf("%02x ", keepDataGroup[OrderGroupNum][i][j]);
				}
				//delete
				printf("\n");
				//[20]�ɂ̓p�P�b�g�f�[�^�������Ă��邩�𔻒f����t���O����
				keepDataGroup[OrderGroupNum][i][20] = 1;

				//�f�[�^�O���[�v�Ō�̃f�[�^�u���b�N���������Ƃ�����
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[OrderGroupNum][i][21] = 1;
					packetSize = PacketNum;
				};
			};
		};

		//�Ō�̃f�[�^�u���b�N������A���S�Ẵf�[�^�u���b�N���������ꍇ
		if (keepDataGroup[OrderGroupNum][PacketNum][21] == 1 || lastPacketFlag == 1) {
			char occupationFlagCount = 0;

			for (unsigned char i = 0; i < PacketNum; i++) {
				if (keepDataGroup[OrderGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				}
			}

			if (occupationFlagCount == (packetSize)) {
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


//�v�f�̒ǉ���
//keepDataGroup.at(0).push_back(1)
//SI0�ɃO���[�v�ԍ��P��ǉ�

//�v�f���̕ύX�i�������j��
//keepDataGroup.resize(0, vector<unsigned char>(0))

//�v�f���擾��
//std::size Numsize = keepDataGroup.size();