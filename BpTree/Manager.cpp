#include "Manager.h"

void Manager::run(const char* command)
{
	fin.open(command);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}
	while (!fin.eof())
	{
		fin.getline(cmd, 32);
		if (!strcmp(cmd, "")) break;
		char* temp = strtok(cmd, " ");
		if (strcmp(temp, "LOAD") == 0) {
			if (LOAD()) {
				flog << "========LOAD=======" << endl;
				printSuccessCode();
			}
			else {
				flog << "========LOAD=======" << endl;
				printErrorCode(100);
			}
		}
		else if (strcmp(temp, "BTLOAD") == 0) {
			if (BTLOAD()) {
				flog << "========BTLOAD=======" << endl;
				printSuccessCode();
			}
			else {
				flog << "========BTLOAD=======" << endl;
				printErrorCode(200);
			}
		}
		else if (strcmp(temp, "PRINT_ITEMLIST") == 0) {
			if (PRINT_ITEMLIST()) {
				flog << "========PRINT_ITEMLIST=======" << endl;
				flog << "Item\tFrequency" << endl;
				fpgrowth->getHeaderTable()->descendingIndexTable();
				fpgrowth->printList();
				flog << "=======================" << endl << endl;
			}
			else {
				flog << "========PRINT_ITEMLIST=======" << endl;
				printErrorCode(300);
			}

		}
		else if (strcmp(temp, "PRINT_FPTREE") == 0) {
			if (PRINT_FPTREE()) {
				flog << "========PRINT_FPTREE=======" << endl;
				flog << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;
				fpgrowth->printTree();
				flog << "=======================" << endl << endl;
			}
			else {
				flog << "========PRINT_FPTREE=======" << endl;
				printErrorCode(400);
			}
		}
		else if (strcmp(temp, "PRINT_MIN") == 0) {

		}
		else if (strcmp(temp, "PRINT_CONFIDENCE") == 0) {

		}
		else if (strcmp(temp, "PRINT_RANGE") == 0) {

		}
		else if (strcmp(temp, "SAVE") == 0) {

		}
		else if (strcmp(temp, "EXIT") == 0) {
			flog << "========EXIT=======" << endl;
			printSuccessCode();
		}
		else {
			flog << "========ERROR=======" << endl;
			flog << "Command Error" << endl;
			flog << "====================" << endl << endl;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	ifstream for_table;
	for_table.open("market.txt");
	if (!for_table || fpgrowth->getTree()->getFrequency())
		return false;
	while (!for_table.eof()) {
		char *table_data = new char[256];
		for_table.getline(table_data, 256);
		char *table_item = strtok(table_data, "\t");
		while (table_item != NULL) {
			fpgrowth->createTable(table_item, 1);
			table_item = strtok(NULL, "\t");
		}
	}
	for_table.close();

	ifstream data;
	data.open("market.txt");
	if (!data || fpgrowth->getTree()->getFrequency())
		return false;
	while (!data.eof()) {
		list<string> item_array;
		char *market = new char[256];
		data.getline(market, 256);
		char *item = strtok(market, "\t");
		while (item != NULL) {
			item_array.push_back(item);
			item = strtok(NULL, "\t");
		}
		fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), item_array, 1);
	}
	data.close();
	return true;
}

bool Manager::BTLOAD()
{
	ifstream data;
	data.open("result.txt");
	if (!data || bptree->getRoot())
		return false;
	while (!data.eof()) {
		set<string> set;
		char *result = new char[256];
		data.getline(result, 256);
		char *item = strtok(result, "\t");
		int key = atoi(item);
		item = strtok(NULL, "\t");
		while (item != NULL) {
			set.insert(item);
			item = strtok(NULL, "\t");
		}
		bptree->Insert(key, set);
	}
	data.close();
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	if (fpgrowth->getHeaderTable()->getindexTable().empty())
		return false;
	else
		return true;
}

bool Manager::PRINT_FPTREE() {
	if (fpgrowth->getTree()->getChildren().empty())
		return false;
	else
		return true;
}

bool Manager::PRINT_MIN(char* item, int min_frequency) {
	return true;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	return true;
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "======== ERROR " << n << " ========" << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

