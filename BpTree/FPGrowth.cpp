#include "FPGrowth.h"

FPGrowth::~FPGrowth() {
	threshold = 2;// 제한
	flog.open("result.txt", ios::app); // result.txt 열어서 내용 저장
	flog.setf(ios::fixed);
	fout->open("log.txt", ios::app);
	fout->setf(ios::fixed);
	fpTree = new FPNode;
	table = new HeaderTable; // fptree랑 headertable생성
}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	FPNode* curNode = new FPNode();
	curNode = root;

	list<pair<int, string>> sorted_array;
	list<pair<int, string>> tmp = table->getindexTable();
	for (auto arr = item_array.begin(); arr != item_array.end(); arr++) {
		for (auto index = tmp.begin(); index != tmp.end(); index++) {
			if (index->second == *arr)
				sorted_array.push_back(pair<int, string>(index->first, index->second));
		}
	}
	sorted_array.sort(greater<pair<int, string>>());
	for (auto it = sorted_array.begin(); it != sorted_array.end(); it++) {
		char item[40];
		strcpy(item, (it->second).c_str());
		FPNode* findNode = curNode->getChildrenNode(it->second);
		if (findNode == NULL) { // item 없을때
			FPNode* newNode = new FPNode();
			newNode->setItem(item);
			newNode->updateFrequency(frequency);
			newNode->setParent(curNode); // 부모노드 지정
			curNode->pushchildren(it->second, newNode); // 새로운 자식노드 추가

			connectNode(table, it->second, newNode); // FPNode랑 table의 dataTable이랑 연결
			curNode = newNode;
		}
		else { // item 있을때
			findNode->updateFrequency(1);
			curNode = findNode;
		}
	}
}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	FPNode* tableNode = new FPNode();
	tableNode = table->getNode(item);
	while (tableNode->getNext() != NULL)
		tableNode = tableNode->getNext();
	tableNode->setNext(node);
}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}

void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; (unsigned)i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {
	list<pair<int, string>> i = table->getindexTable();
	if (table->getindexTable().empty()) return false;
	for (auto it = i.begin(); it != i.end(); it++)
		*fout << it->second << "  " << it->first << endl;

	return true;
}

bool FPGrowth::printTree() {
	table->ascendingIndexTable();
	list<pair<int, string>> i = table->getindexTable();
	FPNode* printNode = new FPNode();
	if (table->getdataTable().empty()) return false;
	for (auto it = i.begin(); it != i.end(); it++) {
		if (it->first >= threshold)
			*fout << "{" << it->second << "," << it->first << "}" << endl;
		else continue;
		printNode = table->getNode(it->second)->getNext();
		while (printNode != NULL) {
			FPNode* parent = printNode;
			while (parent != NULL) {
				if (parent->getFrequency() != NULL)
					*fout << "(" << parent->getItem() << "," << parent->getFrequency() << ") ";
				parent = parent->getParent();
			}
			*fout << endl;
			printNode = printNode->getNext();
		}
	}
	return true;
}

void FPGrowth::saveFrequentPatterns(){

}
