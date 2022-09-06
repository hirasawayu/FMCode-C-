#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

#include "DataBase.h"
#include "DecodeData.h"
#include "KeepDataGroup.h"

std::vector<UnitDataBase> unitDataBase(0);

void DataBase::setData(unsigned char dataTypeNum, unsigned char charData, unsigned int intData, std::string stringData) {
	//データタイプ番号で分岐

	printf("%d\n", charData);
	switch (dataTypeNum){
	case 0:
		DSINum = charData;
		break;
	case 1:
		DdataType = charData;
		break;
	case 2:
		Dparameter = charData;
		break;
	case 3:
		DparameterName = stringData;
		break;
	case 4:
		DchannelNum = charData;
		break;
	case 5:
		DchannelName = stringData;
		break;
	case 6:
		DpageNum = charData;
		break;
	case 7:
		DprefectureNum = charData;
		break;
	case 8:
		DprefectureName = stringData;
		break;
	case 9:
		DXMapNum = charData;
		break;
	case 10:
		DYMapNum = charData;
		break;
	case 11:
		DmeshID = intData;
		break;
	case 12:
		DsecondMesh = intData;
		break;
	case 13:
		Dtime = stringData;
		break;
	case 14:
		DlinkLayer = stringData;
		break;
	case 15:
		unitDataBase.push_back({ 0 });
		unitDataBase[i].unitTypeNum = charData;
		i++;
		break;
	case 16:
		unitDataBase[i].unitTypeName = stringData;
		break;
	case 17:
		unitDataBase[i].unitSize = intData;
		break;
	case 18:
		Dsegment = stringData;
		break;
	case 19:
		DpacketCount = intData;
		break;
	}
	return;
};



