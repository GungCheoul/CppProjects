#include "HeaderTable.h"

HeaderTable::~HeaderTable() {
	indexTable.clear();
	dataTable.clear();
}

void HeaderTable::insertTable(char* item, int frequency) {
	if (indexTable.empty() || find_frequency(item) == NULL) { // index�� ���������, item�� ���� ���� ��
		indexTable.push_back(pair<int, string>(frequency, item));
		FPNode* newNode = new FPNode();
		newNode->setItem(item);
		newNode->updateFrequency(frequency);
		dataTable.insert(pair<string, FPNode*>(item, newNode));
	}
	else { // index�� �� ���������, item�� ���� ���� ��
		list<pair<int, string>>::iterator it_index;
		for (it_index = indexTable.begin(); it_index != indexTable.end(); it_index++) {
			if (it_index->second == item) break;
		}
		it_index->first += 1; // item�� �ش��ϴ� frequency ������Ʈ, �󵵼� 1 ����
		FPNode* update = getNode(item);
		update->updateFrequency(1);
	}
}

int HeaderTable::find_frequency(string item) {
	int frequency = 0;
	list<pair<int, string>>::iterator it;
	for (it = indexTable.begin(); it != indexTable.end(); it++) {
		if (it->second == item) {
			frequency = it->first;
			break;
		}
	}
	if (it == indexTable.end()) return NULL;
	return frequency;
}
