#include "Test.h"
#include <algorithm>
using namespace std;

Test::Test()
{
	testPCA.ReadData(true, testPCA.Getdatapath(), testPCA.GetTrainingData());
	testPCA.Projection(testPCA.Getx(), testPCA.GetTrainingData(), 48);
	Test_Data = new double*[12]();
}
Test::~Test()
{
	for (int i = 0; i < 12; i++) {
		delete[] * (Test_Data + i);
		delete[] * (y + i);
	}
	delete Test_Data;
	delete y;
}
void Test::InputY()
{
	testPCA.GetFile(testpath, ".\\Test", false);
	sort(testpath.begin(), testpath.end(), [&](pair<string, int> a, pair<string, int> b) {return a.second < b.second; });
	testPCA.ReadData(false, testpath, Test_Data);
	y = new double*[12];
	testPCA.Projection(y, Test_Data, 12);
}
void Test::Mapping(int num)
{
	vector<double> result;
	vector<int> idx;
	double tmp;
	int rate = 0;
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 48; j++) {
			tmp = 0.0;
			for (int k = 0; k < num; k++)
				tmp += ((y[i][k] - testPCA.Getx()[j][k]) * (y[i][k] - testPCA.Getx()[j][k]));
			result.push_back(tmp);
			idx.push_back(j);
		}
		sort(idx.begin(), idx.end(), [&](int a, int b) {return result[a] < result[b]; });
		cout << "test : " << i + 1 << ".bmp" << " => matched set_" << (idx[0] / 8) + 1;
		cout << " in " << idx[0] % 8 + 1 << ".bmp" << endl;
		
		result.clear();
		idx.clear();
	}
	cout << endl;
}