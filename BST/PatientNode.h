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

	char * GetName(); // data.txt를 라인 단위로 읽고 PatientNode변수에 저장, 반복문으로 끝날 때까지 Manager.cpp에서
	float GetTemp(); // PatientNode를 거쳐서 Node단위로 환자정보를 queue에 저장
	char GetCough(); // 
	char * GetLoc();

	void SetName(char * name);
	void SetTemp(float temp);
	void SetCough(char cough);
	void SetLoc(char * location);
};
