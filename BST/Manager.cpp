#include "Manager.h"

Manager::Manager()
{
	ds_queue = new queue <PatientNode *>();
	ds_bst = new LocationBST();
	ds_heap = new LocationHeap();
}

Manager::~Manager()
{
	ds_bst = NULL;
	ds_heap = NULL;
}

void Manager::run(const char * command)
{
    fin.open(command);
    flog.open("log.txt", ios::app);
    if(!fin)
    {
        flog << "File Open Error" << endl;
        return;
    }

    char cmd[32];

    while (!fin.eof())
    {
		fin.getline(cmd, 32);
		char * tmp = strtok(cmd, " ");
		if(strcmp(tmp, "LOAD") == 0)
        {
            if(LOAD())
            {
				LocationNode *GWANGJU = new LocationNode();
				LocationNode *DAEGU = new LocationNode();
				LocationNode *SEOUL = new LocationNode();
				LocationNode *BUSAN = new LocationNode();
				LocationNode *DAEJEON = new LocationNode();
				LocationNode *INCHEON = new LocationNode();
				LocationNode *ULSAN = new LocationNode();
				char gwangju[32] = "Gwangju";
				char daegu[32] = "Daegu";
				char seoul[32] = "Seoul";
				char busan[32] = "Busan";
				char daejeon[32] = "Daejeon";
				char incheon[32] = "Incheon";
				char ulsan[32] = "Ulsan";
				GWANGJU->SetLoc(gwangju);
				DAEGU->SetLoc(daegu);
				SEOUL->SetLoc(seoul);
				BUSAN->SetLoc(busan);
				DAEJEON->SetLoc(daejeon);
				INCHEON->SetLoc(incheon);
				ULSAN->SetLoc(ulsan);
				ds_bst->Insert_Location(GWANGJU);
				ds_bst->Insert_Location(DAEGU);
				ds_bst->Insert_Location(SEOUL);
				ds_bst->Insert_Location(BUSAN);
				ds_bst->Insert_Location(DAEJEON);
				ds_bst->Insert_Location(INCHEON);
				ds_bst->Insert_Location(ULSAN);

				char load[] = "LOAD";
				PrintSuccess(load);
            }
            else
				PrintErrorCode(100);
        }
		else if (strcmp(tmp, "ADD") == 0) {
			char *name = strtok(NULL, " ");
			char *temp = strtok(NULL, " ");
			float f_temp = atof(temp);
			char *cough = strtok(NULL, " ");
			char *loc = strtok(NULL, " ");
			if (name == NULL || temp == NULL || cough == NULL || loc == NULL)
				PrintErrorCode(200);
			else {
				PatientNode *addnode = new PatientNode();
				addnode->SetName(name);
				addnode->SetTemp(f_temp);
				addnode->SetCough(cough[0]);
				addnode->SetLoc(loc);
				ds_queue->push(addnode);
				ADD();
			}
		}
		else if (strcmp(tmp, "QPOP") == 0) {
			tmp = strtok(NULL, " ");
			int q_size = atoi(tmp);
			if (q_size > ds_queue->size())
				PrintErrorCode(300);
			else {
				while (q_size > 0) {
					QPOP();
					q_size--;
				}
				char qpop[] = "QPOP";
				PrintSuccess(qpop);
			}
		}
		else if (strcmp(tmp, "SEARCH") == 0) {
			char *name = strtok(NULL, " ");
			if (name == NULL || !SEARCH())
				PrintErrorCode(400);
			else if (SEARCH() && ds_bst->Search(name))
				flog << "============================" << std::endl << endl;
			else
				PrintErrorCode(400);
		}
		else if (strcmp(tmp, "PRINT") == 0) {
			char *print = strtok(NULL, " ");
			if (!strcmp(print, "B") && PRINT()) {
				flog << "========== " << "PRINT" << " ==========" << endl;
				char *order = strtok(NULL, " ");
				if (!strcmp(order, "PRE"))
					ds_bst->Print_PRE();
				else if (!strcmp(order, "IN"))
					ds_bst->Print_IN();
				else if (!strcmp(order, "POST"))
					ds_bst->Print_POST();
				else if (!strcmp(order, "LEVEL"))
					ds_bst->Print_LEVEL();
				flog << "============================" << endl << endl;
			}
			else if (!strcmp(print, "H") && PRINT()) {
				if (ds_heap->GetSize() == 0)
					PrintErrorCode(500);
				else {
					flog << "========== " << "PRINT" << " ==========" << endl;
					flog << "Heap" << endl;
					ds_heap->Print();
					flog << "============================" << endl << endl;
				}
			}
			else
				PrintErrorCode(500);
		}
		else if (strcmp(tmp, "BPOP") == 0) {
			char *name = strtok(NULL, " ");
			if (name == NULL || !BPOP())
				PrintErrorCode(600);
			else if (ds_heap->Insert(ds_bst->Delete(name)) && BPOP()) {
				char bpop[] = "BPOP";
				PrintSuccess(bpop);
			}
			else // no name in bst
				PrintErrorCode(600);
		}
		else if(strcmp(tmp, "EXIT") == 0)
        {
			char exit[] = "EXIT";
            PrintSuccess(exit);
            break;
        }
        else
        {
            flog << "========== ERROR ==========" << endl;
            flog << "Command Error" << endl;
            flog << "===========================" << endl << endl;
        }
    }
    fin.close();
    flog.close();
}

bool Manager::LOAD()
{
	ifstream data;
	data.open("data.txt"); // name, temp, cough, loc
	if (!data || !ds_queue->empty()) {
		std::cout << "========== ERROR ==========" << std::endl;
		std::cout << 100 << std::endl;
		std::cout << "===========================" << std::endl << std::endl;
		return false;
	}
	while (!data.eof()) {
		PatientNode* newnode = new PatientNode();
		char *info = new char[256];
		data.getline(info, 32);
		char *name = strtok(info, " ");
		newnode->SetName(name);
		char *temp = strtok(NULL, " ");
		float f_temp = atof(temp);
		newnode->SetTemp(f_temp);
		char *cough = strtok(NULL, " ");
		newnode->SetCough(cough[0]);
		char *loc = strtok(NULL, " ");
		newnode->SetLoc(loc);
		ds_queue->push(newnode);
	}
	return true;
}

bool Manager::ADD()
{
	PatientNode *node = new PatientNode();
	node = ds_queue->back();
	flog << "========== " << "ADD" << " ==========" << endl;
	flog << node->GetName() << "/" << node->GetTemp() << "/" << node->GetCough() << "/" << node->GetLoc() << endl;
	flog << "============================" << endl << endl;
	return true;
}

bool Manager::QPOP()
{
	PatientNode *node = new PatientNode();
	node = ds_queue->front();
	ds_queue->pop();
	ds_bst->Insert_Patient(node);
	return true;
}

bool Manager::SEARCH()
{
	if (ds_bst->GetRoot() == NULL)
		return false;
	else
		return true;
}

bool Manager::PRINT()
{
	if (ds_bst->GetRoot() == NULL)
		return false;
	else 
		return true;
}

bool Manager::BPOP()
{
	if (ds_bst->GetRoot() == NULL)
		return false;
	else
		return true;
}

void Manager::PrintErrorCode(int num)
{
	flog << "========== ERROR ==========" << endl;
	flog << num << endl;
	flog << "===========================" << endl << endl;
}

void Manager::PrintSuccess(char * act)
{
    flog << "========== " << act << " ==========" << endl;
    flog << "Success" << endl;
    flog << "============================" << endl << endl;
}
