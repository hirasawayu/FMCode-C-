#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"
#include "DecodeData.h"


std::vector<DataGroupCheck> dataGroupCheck;


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
		}

	return keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupCheck[groupPointer].num).at(prefixUnion->prefixData.dataPacketNum).at(20);
}

int KeepDataGroup::addDataBlock(union PrefixUnion* prefixUnion, unsigned char* dataBlock) {

	//�f�[�^�O���[�v�ԍ����`
	unsigned int dataGroupNum = prefixUnion->prefixData.PreChannelNum * 256 + prefixUnion->prefixData.PrePageNum;
	
	//SI�A�f�[�^�O���[�v�A�f�[�^�p�P�b�g�ԍ��������ꍇ�A�v�f��ǉ�����
	//checkedGroupNum�̔z��Ƀf�[�^�O���[�v�ԍ��̗v�f�����邩�𔻒肷��
	auto findGroupNum = std::find(dataGroupCheck.begin(), dataGroupCheck.end(), dataGroupNum);

	//�����ꍇ�͔z��̗v�f�ɒǉ����A�\����dataGroupCheck�ɋL�^����
	if (findGroupNum == dataGroupCheck.end()) {
		
		keepDataGroup.at(prefixUnion->prefixData.SINum).push_back(std::vector<vector<unsigned char>>(0, vector<unsigned char>(22)));
		unsigned char pushNum = dataGroupCheck.size() + 1;
		dataGroupCheck.push_back({ pushNum, dataGroupNum });
	}
	
	// keepDataGroup[22] = {18byte(DataBlock�p), 2byte(SI=D�p), 1byte(DataBlock�������Ă��邩�̃t���O), 1byte(�Ō��DetaBlock�����邩�̃t���O)}
	//���ӁF�e�z��(4�K�w�ڂ͏���)��[0]�Ԗڗv�f�ɂ͉�������Ȃ��i0������
	if (prefixUnion->prefixData.dataPacketNum > keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(prefixUnion->prefixData.dataPacketNum).size()) {
		for (char i = keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(prefixUnion->prefixData.dataPacketNum).size() + 1; i <= prefixUnion->prefixData.SINum; i++) {
			
			keepDataGroup.at(prefixUnion->prefixData.SINum).at(dataGroupNum).at(i).push_back( i );
				//�p�P�b�g�ԍ��ȊO�̐V�����ǉ������ʒu�ɂ̓f�[�^�A�t���O����0������
			if (i != prefixUnion->prefixData.dataPacketNum) {
				keepDataGroup[prefixUnion->prefixData.SINum][dataGroupNum][i][20] = { 0 };
			}
			//�p�P�b�g�ԍ��̈ʒu�ɂ̓f�[�^�u���b�N�����A�t���O��1�ɂ���
			else if (i == prefixUnion->prefixData.dataPacketNum) {
				//[0]~[19]�̓f�[�^�u���b�N����
				for (char j = 0; j < 20; j++) {

					keepDataGroup[prefixUnion->prefixData.SINum][dataGroupNum][i][j] = dataBlock[j];
				}
				//[20]�ɂ̓p�P�b�g�f�[�^�������Ă��邩�𔻒f����t���O����
				keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][20] = { 1 };

				//�f�[�^�O���[�v�Ō�̃f�[�^�u���b�N���������Ƃ�����
				if (prefixUnion->prefixData.endSign == 1) {

					keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][21] = { 1 };
				};


			};
		};

		//�Ō�̃f�[�^�u���b�N������A���S�Ẵf�[�^�u���b�N���������ꍇ
		if (keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][prefixUnion->prefixData.dataPacketNum][21] == 1) {
			char occupationFlagCount = 0;

			for (char i = 1; i <= prefixUnion->prefixData.dataPacketNum; i++) {
				if (keepDataGroup[prefixUnion->prefixData.SINum][prefixUnion->prefixData.dataGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == prefixUnion->prefixData.dataPacketNum) {
				//�S�Ẵf�[�^�u���b�N���������ꍇ��1��Ԃ�
				return 1;
			};
		}
		//�����Ă��Ȃ��ꍇ��0��Ԃ�
		else {
			return 0;
		};
	};
};

//caution DataBlock�̓n�����������A�|�C���^��p���邩
void KeepDataGroup::getDataGroup(union PrefixUnion** prefixUnion) {

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