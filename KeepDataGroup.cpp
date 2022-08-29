#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "KeepDataGroup.h"

int KeepDataGroup::getPacketFlag(struct prefixData &prefix) {
	
	//�v�f���Ƀp�P�b�g�f�[�^�������Ă��邩�̃t���O��Ԃ�
	return keepDataGroup.at(prefix.SINum).at(dataGroupNum).at(dataPacketNum).at(20);
}

int KeepDataGroup::addDataBlock(unsigned char SINum, unsigned char dataGroupNum, unsigned char dataPacketNum, unsigned char dataBlock[20], struct prefixData &prefix) {
	
	//SI�A�f�[�^�O���[�v�A�f�[�^�p�P�b�g�ԍ��������ꍇ�A�v�f��ǉ�����
	// keepDataGroup[22] = {18byte(DataBlock�p), 2byte(SI=D�p), 1byte(DataBlock�������Ă��邩�̃t���O), 1byte(�Ō��DetaBlock�����邩�̃t���O)}
	//���ӁF�e�z��(4�K�w�ڂ͏���)��[0]�Ԗڗv�f�ɂ͉�������Ȃ��i0������
	if (dataPacketNum > keepDataGroup.at(SINum).at(dataGroupNum).at(dataPacketNum).size()) {
		for (char i = keepDataGroup.at(SINum).at(dataGroupNum).at(dataPacketNum).size() + 1; i <= SINum; i++) {
			
			keepDataGroup.at(SINum).at(dataGroupNum).at(i).push_back( i );
				//�p�P�b�g�ԍ��ȊO�̐V�����ǉ������ʒu�ɂ̓f�[�^�A�t���O����0������
			if (i != dataPacketNum) {
				keepDataGroup[SINum][dataGroupNum][i][20] = { 0 };
			}
			//�p�P�b�g�ԍ��̈ʒu�ɂ̓f�[�^�u���b�N�����A�t���O��1�ɂ���
			else if (i == dataPacketNum) {
				//[0]~[19]�̓f�[�^�u���b�N����
				for (char j = 0; j < 20; j++) {

					keepDataGroup[SINum][dataGroupNum][i][j] = dataBlock[j];
				}
				//[20]�ɂ̓p�P�b�g�f�[�^�������Ă��邩�𔻒f����t���O����
				keepDataGroup[SINum][dataGroupNum][i][20] = { 1 };

				//�f�[�^�O���[�v�Ō�̃f�[�^�u���b�N���������Ƃ�����
				if (prefix.endSign == 1) {

					keepDataGroup[SINum][dataGroupNum][i][21] = { 1 };
				};


			};
		};

		//�Ō�̃f�[�^�u���b�N������A���S�Ẵf�[�^�u���b�N���������ꍇ
		if (keepDataGroup[SINum][dataGroupNum][dataPacketNum][21] == 1) {
			char occupationFlagCount = 0;

			for (char i = 1; i <= dataPacketNum; i++) {
				if (keepDataGroup[SINum][dataGroupNum][i][20] == 1) {
					occupationFlagCount += 1;
				};
			};

			if (occupationFlagCount == dataPacketNum) {
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
void KeepDataGroup::getDataGroup(unsigned char &SINum, unsigned char &dataGroupNum, unsigned char &lastPacketNum) {

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