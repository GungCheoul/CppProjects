#include "BpTree.h"

bool BpTree::Insert(int key, set<string> set) {
	FrequentPatternNode* newFPNode = new FrequentPatternNode();
	newFPNode->setFrequency(key);
	newFPNode->InsertList(set);
	if (root == NULL) {
		BpTreeNode* newroot = new BpTreeNode();
		newroot->insertDataMap(key, newFPNode);
		root = newroot;
	}
	else {
		BpTreeNode* Node = searchDataNode(key);
		if (Node != NULL) // fpnode에 set추가, 중복
			Node->getDataMap()->begin()->second->InsertList(set);
		else {
			BpTreeNode* curNode = root;
			while (curNode->getMostLeftChild() == NULL)
				curNode = curNode->getMostLeftChild();
			while (curNode != NULL) {
				if (curNode->getNext() == NULL) {
					curNode->insertDataMap(key, newFPNode);
					if (excessDataNode(curNode)) {
						splitDataNode(curNode);
					}
					break;
				}
				else {
					BpTreeNode* nextNode = curNode->getNext();
					if (curNode->getDataMap()->rbegin()->first < key && nextNode->getDataMap()->begin()->first > key) {
						curNode->insertDataMap(key, newFPNode);
						if (excessDataNode(curNode)) {
							splitDataNode(curNode);
						}
						break;
					}
				}
				curNode = curNode->getNext();
			}
		}
	}
	return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
	BpTreeNode* pCur = root;
	while (pCur->getMostLeftChild() == NULL) // 제일 왼쪽, DataNode로 이동
		pCur = pCur->getMostLeftChild();
	while (pCur != NULL) { // DataNode의 끝까지 확인
		if (pCur->getDataMap()->find(n) == pCur->getDataMap()->end())
			pCur = pCur->getNext();
		else
			return pCur;
	}
	return pCur;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	double split = ceil((order - 1) / 2.0 + 1);
	map<int, FrequentPatternNode*>::iterator it = pDataNode->getDataMap()->begin();
	BpTreeIndexNode* indexNode = new BpTreeIndexNode();
	BpTreeDataNode* dataNodeL = new BpTreeDataNode();
	BpTreeDataNode* dataNodeR = new BpTreeDataNode();
	for (int i = 1; i < split; i++) {
		dataNodeL->insertDataMap(it->first, it->second);
		it++;
	}
	int key = it->first;
	while (it != pDataNode->getDataMap()->end()) {
		dataNodeR->insertDataMap(it->first, it->second);
		it++;
	}
	
	if (pDataNode->getParent() == NULL) { // root없을떄
		indexNode->insertIndexMap(key, dataNodeR);
		indexNode->setMostLeftChild(dataNodeL);
		dataNodeL->setNext(dataNodeR);
		dataNodeR->setPrev(dataNodeL);
		dataNodeL->setParent(indexNode);
		dataNodeR->setParent(indexNode);
		root = indexNode;
	}
	else {
		pDataNode->getParent()->insertIndexMap(key, dataNodeR);
		dataNodeL->setNext(dataNodeR);
		dataNodeR->setPrev(dataNodeL);
		dataNodeL->setParent(pDataNode->getParent());
		dataNodeR->setParent(pDataNode->getParent());
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	double split = ceil((order - 1) / 2.0 + 1);
	BpTreeIndexNode* indexNodeL = new BpTreeIndexNode();
	BpTreeIndexNode* indexNodeR = new BpTreeIndexNode();
	BpTreeIndexNode* indexNodeM = new BpTreeIndexNode();
	map<int, BpTreeNode*>::iterator it = pIndexNode->getIndexMap()->begin();
	int keyL = it->first;	BpTreeNode* Lnode = it->second;	it++;
	int keyR = it->first;	BpTreeNode* Rnode = it->second;	it++;
	int keyM = it->first;	BpTreeNode* Mnode = it->second;
	if (pIndexNode->getParent() == NULL) { // root없을때
		indexNodeL->insertIndexMap(keyL, Lnode);
		indexNodeL->setMostLeftChild(pIndexNode->getMostLeftChild());
		indexNodeR->insertIndexMap(keyR, Rnode);	
		indexNodeR->setMostLeftChild(Mnode);
		indexNodeM->insertIndexMap(keyM, Mnode);
		indexNodeM->setMostLeftChild(indexNodeL);
		indexNodeL->setParent(indexNodeM);
		indexNodeR->setParent(indexNodeM);
		root = indexNodeM;
	}
	else {
		pIndexNode->getParent()->insertIndexMap(keyM, Mnode);
		indexNodeR->insertIndexMap(keyR, Rnode);
		indexNodeR->setMostLeftChild(Mnode);
		indexNodeR->setParent(pIndexNode->getParent());
	}
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > (unsigned)(order - 1)) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > (unsigned)(order - 1)) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::printConfidence(string item, double item_frequency, double min_frequency)
{

	return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{

	return true;
}
bool BpTree::printRange(string item, int min, int max) {

	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		if (temp != item) *fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}
