#include "PCA.h"
#include <atlstr.h> // cstring
#include <io.h> // file directory

PCA::PCA()
{
	Training_Data = new double*[48]();
	m = new double[10304]();
	GetFile(datapath, ".\\Training", true);
	x = new double*[48];
}
PCA::~PCA()
{
	for (int i = 0; i < 48; i++) {
		delete[] * (Training_Data + i);
		delete[] * (x + i);
	}
	delete x;
	delete Training_Data;
	delete m;
}
void PCA::GetFile(vector<pair<string, int>>& list, string path, bool directory)
{
	string tmp = path + string("\\*.*");
	struct _finddata_t fd;
	intptr_t find = _findfirst(tmp.c_str(), &fd);
	if (find != -1L) {
		do {
			int check;
			if (fd.attrib & _A_SUBDIR) check = 0;
			else check = 1;
			if (check == 0) {
				if (directory) {
					if (fd.name[0] != '.') {
						tmp = path + string("\\") + string(fd.name);
						GetFile(list, tmp, directory);
					}
				}
			}
			else {
				tmp = path + string("\\") + string(fd.name);
				CString str = tmp.c_str();
				str.TrimRight(_T(".bmp"));
				int pos = str.ReverseFind('/');
				str.Delete(0, pos + 1);
				int i = 0;
				for (i = 0; i < str.GetLength(); i++) {
					if (isdigit(str[i]) != 0) break;
				}
				str.Delete(0, i);
				int integer = _ttoi(str);
				list.push_back(make_pair(tmp, integer));
			}
		} while (!_findnext(find, &fd));
		_findclose(find);
	}
}
void PCA::ReadData(bool X, vector<pair<string, int>> datapath, double **data)
{
	int i = 0;
	for (auto path : datapath) {
		FILE *file = fopen(path.first.c_str(), "rb");
		fread(&BF, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&BI, sizeof(BITMAPINFOHEADER), 1, file);
		fseek(file, BF.bfOffBits, SEEK_SET);
		unsigned char *image = new unsigned char[BI.biSizeImage]();
		fread(image, sizeof(char), BI.biSizeImage, file);

		*(data + i) = new double[10304]();
		for (int r = 111; r >= 0; r--) {
			for (int c = 0; c < 92; c++)
				data[i][(111 - r) * 92 + c] = image[r * 92 * 3 + (c * 3)];
		}
		delete image;
		fclose(file);
		i++;
	}
	Average(X);
	if (X) GetEigen();
}
void PCA::Average(bool X)
{
	if (!X) return;
	for (int i = 0; i < 48; i++) {
		for (int j = 0; j < 112; j++) {
			for (int k = 0; k < 92; k++)
				m[j * 92 + k] += Training_Data[i][j * 92 + k];
		}
	}
	for (int i = 0; i < 10304; i++)
		m[i] /= 48;
}
void PCA::Normalization(double **data, int num)
{
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < 112; j++) {
			for (int k = 0; k < 92; k++)
				data[i][j * 92 + k] -= m[j * 92 + k];
		}
	}
}
void PCA::GetEigen()
{
/*
	MatrixXd dataMatrix = MatrixXd(48, 10304);
	for (int i = 0; i < 48; i++) {
		for (int j = 0; j < 112; j++) {
			for (int k = 0; k < 92; k++)
				dataMatrix(i, j * 92 + k) = Training_Data[i][j * 92 + k];
		}
	}
	MatrixXd covariance = MatrixXd(10304, 10304);
	MatrixXd transpose = dataMatrix.transpose();
	for (int i = 0; i < 10304; i++) {
		for (int j = 0; j < 10304; j++)
			covariance(i, j) = dataMatrix.transpose().row(i) * dataMatrix.col(j);
	}
	covariance /= 48;
	EigenSolver<MatrixXd> EigenVec(covariance);
	MatrixXd eigenVector = MatrixXd(10304, 10304);
	eigenVector = EigenVec.eigenvectors().real(); */

	EigenVector = new double*[10]();
	char tmp[128] = { 0, };
	for (int i = 0; i < 10; i++) {
		*(EigenVector + i) = new double[10304]();
		sprintf(tmp, "EigenVector%d.txt", i + 1);
		FILE *file = fopen(tmp, "r");
		char eigen[128];
		for (int j = 0; j < 10304; j++) {
			fgets(eigen, 128, file);
			EigenVector[i][j] = atof(eigen);
		}
		fclose(file);
	}
}
void PCA::Projection(double **proj, double **data, int num)
{	
	Normalization(data, num);
	for (int i = 0; i < num; i++) {
		*(proj + i) = new double[10]();
		for (int r = 0; r < 10; r++) {
			for (int c = 0; c < 10304; c++)
				proj[i][r] += EigenVector[r][c] * data[i][c]; 
		}
	}
}