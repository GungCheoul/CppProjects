#pragma once
#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

class FPGrowth
{
private:
	int threshold;
	FPNode* fpTree; // fptree, 자식노드 -> 상품이름, 하위 fpnode
	HeaderTable* table; // index랑 data table
	map<set<string>, int> frequenctPatterns;
	ofstream* fout;
	ofstream flog;
public:
	FPGrowth(ofstream *fout, int threshold = 3) { // FPGrowth 생성자
		this->threshold = threshold; // 제한
		flog.open("result.txt", ios::app); // result.txt 열어서 내용 저장
		flog.setf(ios::fixed);
		fpTree = new FPNode();  table = new HeaderTable(); // fptree랑 headertable생성
		this->fout = fout;
	}
	~FPGrowth();
	void createTable(char* item, int frequency) { table->insertTable(item, frequency); } // inserTable로 table삽입, 생성
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency);
	void connectNode(HeaderTable* table, string item, FPNode* node);

	
	void frequenctPatternSetting() {
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);
	int item_frequency(string item) {return table->find_frequency(item);}
	FPNode* getTree() { return fpTree; }
	HeaderTable* getHeaderTable() { return table; }

	bool printList();
	bool printTree();
	void saveFrequentPatterns();

	};


