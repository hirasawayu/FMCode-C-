#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "main.h"
#include "DecodeData.h"

int main()
{
	//KeepDataGroup�̃I�u�W�F�N�g�𐶐�
	DecodeData decode;

	//�ǂݍ��ރo�C�i���t�@�C���̎w��
	//caution Drag/Drop�Ńt�@�C�����w�肷����@��T��
	std::ifstream ifs("", std::ios::in | std::ios::binary);

	unsigned char packet[22];

	ifs.read(reinterpret_cast<char*>(std::addressof(packet)), 22);

	//DecodeData�N���X�ɓǂݍ��񂾃p�P�b�g�f�[�^��n��
	
	decode.callDecodeData();


}