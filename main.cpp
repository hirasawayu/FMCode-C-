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
	//KeepDataGroupのオブジェクトを生成
	DecodeData decode;

	//読み込むバイナリファイルの指定
	//caution Drag/Dropでファイルを指定する方法を探す
	std::ifstream ifs("", std::ios::in | std::ios::binary);

	unsigned char packet[22];

	ifs.read(reinterpret_cast<char*>(std::addressof(packet)), 22);

	//DecodeDataクラスに読み込んだパケットデータを渡す
	
	decode.callDecodeData();


}