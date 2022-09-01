#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <stdlib.h>

struct DataGroupCheck
{
	unsigned char num;
	unsigned int checkedGroupNum;
};


int main() {
	std::vector<DataGroupCheck> dataGroupCheck{};

	dataGroupCheck.push_back({ 1,0xAAAA });
	dataGroupCheck.push_back({ 2,0xBBBB });
	dataGroupCheck.push_back({ 3,0xCCCC });
	dataGroupCheck.push_back({ 4,0xDDDD });
	dataGroupCheck.push_back({ 5,0xEEEE });

	unsigned int dataGroupNum = 0xEEEE;
	unsigned char number = 0;;
	for (char i = 0; i < 5; i++) {
		if (dataGroupCheck[i].checkedGroupNum == dataGroupNum) {
			number = dataGroupCheck[i].num;
			printf("i: %d\n", number);
		}

	}
	std::cout << dataGroupCheck.size() << std::endl;
		printf("num: %d\n", dataGroupCheck[3].num);
		printf("int: %x\n", dataGroupCheck[3].checkedGroupNum);

		return 0;
}


