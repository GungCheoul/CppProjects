#pragma once
#include <iostream>
#include <windows.h> // read bmp
#include <string>
#include <vector>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

class PCA
{
private:
	BITMAPFILEHEADER BF;
	BITMAPINFOHEADER BI;
	vector<pair<string, int>> datapath;
	double **Training_Data;
	double *m;
	double **EigenVector;
	double **x;

public:
	PCA();
	~PCA();
	void GetFile(vector<pair<string, int>>& list, string path, bool directory);
	void ReadData(bool X, vector<pair<string, int>> datapath, double **data);
	void Average(bool X);
	void Normalization(double **data, int num);
	void GetEigen();
	void Projection(double **proj, double **data, int num);

	vector<pair<string, int>> Getdatapath() { return datapath; }
	double **GetTrainingData() { return Training_Data; }
	double **Getx() { return x; }
};