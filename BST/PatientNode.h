#pragma once
#include <cstring>

class PatientNode
{
private:
	char * Name;
	float Temperature;
	char Cough;
	char * Location;

public:
	PatientNode();
	~PatientNode();

	char * GetName(); // data.txt�� ���� ������ �а� PatientNode������ ����, �ݺ������� ���� ������ Manager.cpp����
	float GetTemp(); // PatientNode�� ���ļ� Node������ ȯ�������� queue�� ����
	char GetCough(); // 
	char * GetLoc();

	void SetName(char * name);
	void SetTemp(float temp);
	void SetCough(char cough);
	void SetLoc(char * location);
};
