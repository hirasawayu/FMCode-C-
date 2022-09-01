#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "main.h"
#include "DecodeData.h"

int main()
//int main(int argc, char* argv[])
{
	//KeepDataGroupのオブジェクトを生成
	DecodeData decode;

	//読み込むバイナリファイルの指定
	//caution Drag/Dropでファイルを指定する方法を探す
	//std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
	std::ifstream ifs("6_2_SKY_20180225_1003.dpk");

	// 開けなかったらエラー
	if (!ifs)
	{
		std::cout << "Error! File can not be opened." << std::endl;
		return 0;
	}
	unsigned char packet[22] = {};

	while (!ifs.eof()) {
		ifs.read(reinterpret_cast<char*>(packet), 22);

		//DecodeDataクラスに読み込んだパケットデータを渡す
		decode.callDecodeData(packet);
	}
	return 0;

	 
}