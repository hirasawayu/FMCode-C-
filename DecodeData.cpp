#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "DecodeData.h"
#include "KeepDataGroup.h"

//KeepDataGroup�̃I�u�W�F�N�g�𐶐�
KeepDataGroup keep;



struct PrefectureList prefectureList[47]
{
	{1, "(�k�C��)"},
	{2, "(�X��)"},
	{3, "(��茧)"},
	{4, "(�{�錧)"},
	{5, "(�H�c��)"},
	{6, "(�R�`��)"},
	{7, "(������)"},
	{8, "(��錧)"},
	{9, "(�Ȗ،�)"},
	{10, "(�Q�n��)"},
	{11, "(��ʌ�)"},
	{12, "(��t��)"},
	{13, "(�����s)"},
	{14, "(�_�ސ쌧)"},
	{15, "(�V����)"},
	{16, "(�x�R��)"},
	{17, "(�ΐ쌧)"},
	{18, "(���䌧)"},
	{19, "(�R����)"},
	{20, "(���쌧)"},
	{21, "(�򕌌�)"},
	{22, "(�É���)"},
	{23, "(���m��)"},
	{24, "(�O�d��)"},
	{25, "(���ꌧ)"},
	{26, "(���s�{)"},
	{27, "(���{)"},
	{28, "(���Ɍ�)"},
	{29, "(�ޗǌ�)"},
	{30, "(�a�̎R��)"},
	{31, "(���挧)"},
	{32, "(������)"},
	{33, "(���R��)"},
	{34, "(�L����)"},
	{35, "(�R����)"},
	{36, "(������)"},
	{37, "(���쌧)"},
	{38, "(���Q��)"},
	{39, "(���m��)"},
	{40, "(������)"},
	{41, "(���ꌧ)"},
	{42, "(���茧)"},
	{43, "(�F�{��)"},
	{44, "(�啪��)"},
	{45, "(�{�茧)"},
	{46, "(��������)"},
	{47, "(���ꌧ)"},
};

unsigned char DecodeData::bitreverse(unsigned char v)
{
	v = (v & 0x55555555) << 1 | (v >> 1 & 0x55555555);
	v = (v & 0x33333333) << 2 | (v >> 2 & 0x33333333);
	v = (v & 0x0f0f0f0f) << 4 | (v >> 4 & 0x0f0f0f0f);
	v = (v & 0x00ff00ff) << 8 | (v >> 8 & 0x00ff00ff);
	v = (v & 0x0000ffff) << 16 | (v >> 16 & 0x0000ffff);
	return v;
}

void DecodeData::setPrefix(unsigned char packetData[22]) {

	//�t�����̏ꍇ
	if (packetData[1] == 'B') {

		//�e�z���bit���t���ɂ���
		unsigned char p1 = bitreverse(packetData[0]);
		unsigned char p2 = packetData[1];

		//�t���ɂ����l������z���p��
		unsigned char prefix[2];

		//�p�ӂ����z��ɒl���R�s�[
		memset(prefix, p1, 1);
		memset(&prefix[1], p2, 1);

	}

	else {
		
		unsigned char p1 = bitreverse(packetData[0]);
		unsigned char p2 = packetData[1];
		unsigned char p3 = packetData[2];
		unsigned char p4 = packetData[3];

		unsigned char prefix[4];

		memset(prefix, p1, 1);
		memset(&prefix[1], p2, 1);
		memset(&prefix[2], p3, 1);
		memset(&prefix[3], p4, 1);

		//SI�A�O���[�v�A�p�P�b�g�ԍ��ɑΉ�����ʒu�Ƀf�[�^�p�P�b�g���18bit���i�[
		unsigned char* pdataBlock = dataBlock;
		pdataBlock = &packetData[4];
	}
}
void DecodeData::createDataGroup(std::vector<unsigned char>& dataGroup) {

	keep.getDataGroup(prefixData);

	for (int i = 0; i > packetNum; i++) {

		std::vector<unsigned char> dataGroup =




	}

}

unsigned char DecodeData::getDataGroupLink(std::vector<unsigned char> &dataGroup, unsigned char &bp) {
	//1000 0000����p�����}�X�N����
	if ((dataGroup[bp] & 0x80) == 0x80) {
		return 1;
	}
	else {
		return 0;
	}
}


unsigned char  DecodeData::getDataGroupType(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	
	if (dataGroup[bp] == 0x1E) {
		return 0;
	}
	else if (dataGroup[bp] == 0x1F) {
		return 1;
	}
}

unsigned char  DecodeData::getParameter(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0000 1111����p�����}�X�N����
	unsigned char para = (dataGroup[bp] & 0x0F);
	return para;
}

void  DecodeData::getChannelInfo(unsigned char &channelNum, std::string &channelName, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	channelNum == dataGroup[bp];
	if (channelNum == 0) {
		std::string channelName = "(���ڎ�/�ŏ��̉��)";
	}
	else if (channelNum >= 1 && channelNum < 20) {
		std::string channelName = "(�j���[�X)";
	}
	else if (channelNum >= 20 && channelNum < 40) {
		std::string channelName = "(��ʏ��ԑg)";
	}
	else if (channelNum >= 40 && channelNum < 80) {
		std::string channelName = "(��ʏ��)";
	}
	else if (channelNum >= 80 && channelNum < 100) {
		std::string channelName = "(�ԑg���)";
	}
	else if (channelNum >= 100 && channelNum < 150) {
		std::string channelName = "(�C�ۏ��)";
	}
	else if (channelNum >= 150 && channelNum < 250) {
		std::string channelName = "(���̑�)";
	}
	return;
}

//���e�X�V�͕s�v�H
//unsigned char  DecodeData::getUpdateNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
//}
unsigned char  DecodeData::getPageNum(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0011 1111����p�����}�X�N����
	unsigned char page = (dataGroup[bp] & 0x3F);
	return page;
}

unsigned char  DecodeData::getMacroSign(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//4���E�r�b�g�V�t�g(1111 0000)
	unsigned char macro = (dataGroup[bp] >> 4);
	return macro;
}

unsigned char  DecodeData::getPrefecture(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0011 1111����p�����}�X�N����
	unsigned char prefec = (dataGroup[bp] & 0x3F);
	return prefec;
}

void  DecodeData::getXMapAddress(unsigned char& XMapNum, unsigned char& XMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//3���E�r�b�g�V�t�g(1111 1000)
	XMapNum = (dataGroup[bp] >> 3);

	//0000 0111���}�X�N
	XMeshNum = (dataGroup[bp] & 0x07);

	return;
}

void  DecodeData::getYMapAddress(unsigned char& YMapNum, unsigned char& YMeshNum, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//3���E�r�b�g�V�t�g(1111 1000)
	YMapNum = (dataGroup[bp] >> 3);

	//0000 0111���}�X�N
	YMeshNum = (dataGroup[bp] & 0x07);

	return;
}

//caution unsigned char�̘A��
int DecodeData::getMeshID(unsigned char XMapNum, unsigned char YMapNum) {
	int meshID = ;
	return meshID;
}

int DecodeData::getSecondMesh(unsigned char XMapNum, unsigned char YMapNum, unsigned char XMeshNum, unsigned char YMeshNum) {
	int secondMesh = (YMapNum + 37) * 10000 + (XMapNum + 22) * 100 + YMeshNum * 10 + XMeshNum;
	return secondMesh;
}

unsigned char  DecodeData::getTime(std::string &time, std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//0000 1000���}�X�N
	unsigned char timeFlag = (dataGroup[bp] & 0x08);
	if (timeFlag == 1) {
		unsigned char hour = ((dataGroup[bp] & 0x07) << 3) + ((dataGroup[bp + 1] & 0xC0) >> 6);
		unsigned char minute = (dataGroup[bp + 1] & 0x3F);

		time = hour + ":" + minute;
	}
	return timeFlag;
}

std::string DecodeData::getLinkLayer(std::vector<unsigned char>& dataGroup, unsigned char& bp) {
	//1100 0000���}�X�N
	unsigned char linkLayerNum = ((dataGroup[bp] & 0xC0) >> 6);
	switch (linkLayerNum) {
	case 1:
		return "�L��";
	case 2:
		return "����";
	case 3:
		return "����";
	}
}

unsigned char  DecodeData::getEndGroupFlag() {

}



unsigned char  DecodeData::getDataUnitSize() {

}

unsigned char  DecodeData::getDataUnitType() {


}

unsigned char  DecodeData::getSegmentSize() {

}

unsigned char  DecodeData::getSegmentType() {

}





//�f�R�[�h�N���X��main����
void  DecodeData::callDecodeData(struct PrefixData &prefixData, struct PrefectureList &prefectureList)
{
	while (1) {

		unsigned char dataPacket[22];

		setPrefix(dataPacket);

		//caution SINum�������p�̂���擾�ł��Ă��邩���m�F����


		packetFlag = keep.getPacketFlag(prefixData);

		//�w��̈ʒu�Ƀp�P�b�g�f�[�^�������Ă��Ȃ��ꍇ
		if (packetFlag = 0 || NULL) {


			//KeepDataBlock�N���X�̎w��̈ʒu�Ƀf�[�^�u���b�N���i�[����
			ret = keep.addDataBlock(SINum, dataGroupNum, dataPacketNum, dataBlock, prefix);

			//�f�[�^�u���b�N�������܂Ń��[�v
			if (ret == 1) {
				break;

			}
		}
	}

	std::vector<unsigned char> dataGroup;
	//caution �쐬�����f�[�^�O���[�v������z����ǂ�����Ă����Ă��邩�A�f�[�^�O���[�v�쐬
	createDataGroup(dataGroup);

	//�Q�Ƃ���o�C�g�ʒu(BytePlace)���L��
	unsigned char bp = 0;

	while (1) {

		//BD2 DGL�擾
		bp++;
		dataGroupLink = getDataGroupLink(dataGroup, bp);

		//DB(n), +2�@�O���[�v�\���擾
		bp += 2;
		dataGroupType = getDataGroupType(dataGroup, bp);

		//DB(n+1)�@�p�����[�^�擾
		bp++;
		//RS(�f�[�^�w�b�_)�̏ꍇ
		if (dataGroupType = 0) {

			parameter = getParameter(dataGroup, bp);

			//DB(n+2)�@�ԑg�ԍ��擾
			bp++;
			if (parameter == 0 || 1 || 2 || 3 || 4 || 8 || 9 || 10 || 11) {
				getChannelInfo(channelNum, channelName, dataGroup, bp);
			}
			else if (parameter = 6) {
				break;
			}

			//DB(n+3) ���e�X�V�A�y�[�W����/�y�[�W�ԍ��擾
			bp++;
			if (parameter == 0 || 1 || 2 || 3 || 4 || 8 || 9 || 10 || 11) {
				pageNum = getPageNum(dataGroup, bp);
			}

			//�ԑg���ʃ}�N�����
			//DB(n+4) �ԑg���ʃ}�N���W���擾
			bp++;
			if (parameter == 4 || 5) {
				macroSign = getMacroSign(dataGroup, bp);
				//�}�N���W��������1��������2�o�C�g�̕���
				if (macroSign == 2) {
					bp += 3;
				}
				if (macroSign == 3) {
					bp += 2;
				}

				//�\��A�͂����ŏI��
				if (parameter == 4) {
					break;
				}

				//(n+6/7) �s���{���擾
				prefectureNum = getPrefecture(dataGroup, bp);
				prefectureName = prefectureList.prefectureName[prefectureNum];

				//�ԑg���ʃ}�N���͂����ŏI��
				bp += 7;
				break;
			}

			//�ԑg�f�[�^�w�b�_�̏ꍇ
			if (parameter == 0 || 1) {
				bp += 3;
			}
			//�y�[�W�f�[�^�w�b�_�̏ꍇ
			if (parameter == 2 || 3) {
				bp = +4;
			}
			//�\��A�͂����ŏI��
			if (parameter == 0 || 2) {
				break;
			}

			//�s���{���擾
			prefectureNum = getPrefecture(dataGroup, bp);
			prefectureName = prefectureList.prefectureName[prefectureNum];

			//(n += 2)X���W�@�n�}�ԍ��A�񎟃��b�V���擾
			bp += 2;
			getXMapAddress(XMapNum, XMeshNum, dataGroup, bp);

			//(n += 1)Y���W�@�n�}�ԍ��A�񎟃��b�V���擾
			bp++;
			getYMapAddress(YMapNum, YMeshNum, dataGroup, bp);

			//MeshID�A�񎟃��b�V���擾
			meshID = getMeshID(XMapNum, YMapNum);

			secondMesh = getSecondMesh(XMapNum, YMapNum, XMeshNum, YMeshNum);

			//(n += 2)�ԑg�f�[�^�w�b�_�͂����ŏI��
			bp += 2;
			if (parameter == 0) {
				break;
			}
			//���񋟎����擾
			timeFlag = getTime(time, dataGroup, bp);

			//(n += 1)�����N���C���擾
			linkLayer == getLinkLayer(dataGroup, bp);

			//(n += 2)�y�[�W�f�[�^�w�b�_�I��
			bp += 2;
			break;
		}

		//US(���j�b�g�f�[�^)�̏ꍇ
		else if (dataGroupType == 1) {

		}

	}
}
