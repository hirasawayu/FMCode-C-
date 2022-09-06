#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>

struct UnitDataBase {
	unsigned char unitTypeNum = 0;
	std::string unitTypeName = "";
	unsigned int unitSize = 0;

};

class DataBase {
private:

	int i = 0;

	unsigned char DSINum = 0;
	unsigned char DdataType = 0;
	unsigned char Dparameter = 0;
	std::string DparameterName = "";
	unsigned char DchannelNum = 0;

	std::string DchannelName = "";
	unsigned char DpageNum = 0;
	unsigned char DprefectureNum = 0;
	std::string DprefectureName = "";
	unsigned char DXMapNum = 0;

	unsigned char DYMapNum = 0;
	unsigned int DmeshID = 0;
	unsigned int DsecondMesh = 0;
	std::string Dtime = "";
	std::string DlinkLayer = "";

	std::string Dsegment = "";
	unsigned int DpacketCount = 0;

public:
	void setData(unsigned char dataTypeNum, unsigned char charData, unsigned int intData, std::string stringData);

	std::vector <UnitDataBase> getUnitData();

	
};