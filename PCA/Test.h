#pragma once
#include "PCA.h"
#include <vector>
using namespace std;

class Test
{
private:
	PCA testPCA;
	vector<pair<string, int>> testpath;
	double **Test_Data;
	double **y;

public:
	Test();
	~Test();
	void InputY();
	void Mapping(int num);
};