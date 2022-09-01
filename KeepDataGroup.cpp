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
	//vector<vector<�v�f�̌^>> �ϐ���(�v�f��1, vector<�v�f�̌^>(�v�f��2, �����l));
vector<vector<vector<vector<unsigned char>>>> keepDataGroup(13, vector<vector<vector<unsigned char>>>(0, vector<vector<unsigned char>>(0, vector<unsigned char>(22))));


int KeepDataGroup::getPacketFlag(union PrefixUnion* prefixUnion) {
	//�f�[�^�O���[�v�ԍ����`
	unsigned int dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;

	//�v�f���Ƀp�P�b�g�f�[�^�������Ă��邩�̃t���O��Ԃ�
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
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

	//�f�[�^�O���[�v�ԍ����`
	dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
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

	// keepDataGroup[22] = {18byte(DataBlock�p), 2byte(SI=D�p), 1byte(DataBlock�������Ă��邩�̃t���O), 1byte(�Ō��DetaBlock�����邩�̃t���O)}
	//���ӁF�e�z��(4�K�w�ڂ͏���)��[0]�Ԗڗv�f�ɂ͉�������Ȃ��i0������
	if (packetNum > keepDataGroup.at(SINum).at(pushNum).at(packetNum).size()) {
		for (unsigned char i = keepDataGroup.at(SINum).at(pushNum).at(packetNum).size() + 1; i <= SINum; i++) {

			keepDataGroup.at(SINum).at(pushNum).at(i).push_back(i);
			//�p�P�b�g�ԍ��ȊO�̐V�����ǉ������ʒu�ɂ̓f�[�^�A�t���O����0������
			if (i != packetNum) {
				keepDataGroup[SINum][pushNum][i][20] = { 0 };
			}
			//�p�P�b�g�ԍ��̈ʒu�ɂ̓f�[�^�u���b�N�����A�t���O��1�ɂ���
			else if (i == packetNum) {
				//[0]~[19]�̓f�[�^�u���b�N����
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][pushNum][i][j] = dataBlock[j];
				}
				//[20]�ɂ̓p�P�b�g�f�[�^�������Ă��邩�𔻒f����t���O����
				keepDataGroup[SINum][pushNum][i][20] = { 1 };

				//�f�[�^�O���[�v�Ō�̃f�[�^�u���b�N���������Ƃ�����
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup.at(SINum).at(pushNum).at(i).at(21) = {1};
					packetSize = packetNum;
				};


			};
		};

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
		}
		//�����Ă��Ȃ��ꍇ��0��Ԃ�
		else {
			lastPacketFlag = 1;
			return 0;
		}
	}
}
//caution DataBlock�̓n�����������A�|�C���^��p���邩
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


//�e�v�f�̃A�N�Z�X��
//keepDataGroup.at(0).at(1).at(2).at(3)

//�v�f�̒ǉ���
//keepDataGroup.at(0).push_back(1)
//SI0�ɃO���[�v�ԍ��P��ǉ�

//�v�f���̕ύX�i�������j��
//keepDataGroup.resize(0, vector<unsigned char>(0))

//�v�f���擾��
//std::size Numsize = keepDataGroup.size();