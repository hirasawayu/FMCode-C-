#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "DecodeData.h"
#include "Documentation.h"
#include "DataBase.h"

int main()
//int main(int argc, char* argv[])
{
	//KeepDataGroup�̃I�u�W�F�N�g�𐶐�
	DecodeData decode;
	
	//�ǂݍ��ރo�C�i���t�@�C���̎w��
	//caution Drag/Drop�Ńt�@�C�����w�肷����@��T��
	//std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
	std::ifstream ifs("6_2_SKY_20180225_1003.dpk", std::ios::in | std::ios::binary);

	// �J���Ȃ�������G���[
	if (!ifs)
	{
		std::cout << "Error! File can not be opened." << std::endl;
		ifs.close();
		return 0;
	}
	unsigned char packet[22] = {};

	while (!ifs.eof()) {
		ifs.read(reinterpret_cast<char*>(packet), 22);

		
		
		//DecodeData�N���X�ɓǂݍ��񂾃p�P�b�g�f�[�^��n��
		decode.callDecodeData(packet);
	}

	return 0;
}