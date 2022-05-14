#include "PatientBST.h"

PatientBST::PatientBST()
{
	Root = NULL;// new PatientBSTNode();
}

PatientBST::~PatientBST()
{
	Root = NULL;// new PatientBSTNode();
}

PatientBSTNode * PatientBST::GetRoot()
{
	return Root;
}

bool PatientBST::Insert(PatientNode * node) // Patient_Queue에서 pop된 node -> PatientBSTNode로
{
    //QPOP
	PatientBSTNode* newnode = new PatientBSTNode();
	newnode->SetName(node->GetName()); // name
	if (node->GetTemp() >= 37.5 && node->GetCough() == 'Y') // disease + or -
		newnode->SetDisease('+');
	else
		newnode->SetDisease('-');

	if (Root == NULL)
		Root = newnode;
	else {
		PatientBSTNode* current = new PatientBSTNode();
		PatientBSTNode* parent = new PatientBSTNode();
		current = Root;
		while (current != NULL) {
			parent = current;
			if (strcmp(current->GetName(), newnode->GetName()) == 0) return false;
			if (strcmp(current->GetName(), newnode->GetName()) > 0)
				current = current->GetLeft();
			else
				current = current->GetRight();
		}
		if (strcmp(parent->GetName(), newnode->GetName()) > 0)
			parent->SetLeft(newnode);
		else
			parent->SetRight(newnode);
	}
	return true;
}

bool PatientBST::Search(char * name)
{
    //SEARCH
	PatientBSTNode* current = new PatientBSTNode();
	current = Root;
	while (current != NULL) {
		if (strcmp(current->GetName(), name) == 0) {
			ofstream flog;
			flog.open("log.txt", ios::app);
			flog << "========== " << "SEARCH" << " ==========" << endl;
			flog << current->GetName() << "/" << current->GetDisease() << endl;
			return true;
		}
		else if (strcmp(current->GetName(), name) > 0)
			current = current->GetLeft();
		else
			current = current->GetRight();
	}
	if (current == NULL) return false; //
}

bool PatientBST::Delete(char * name)
{
    //BPOP
	PatientBSTNode* current = new PatientBSTNode();
	PatientBSTNode* parent = new PatientBSTNode();
	PatientBSTNode* leftright = new PatientBSTNode();
	current = Root;

	while (current != NULL) {
		parent = current;
		if (!strcmp(current->GetName(), name))
			break;
		if (strcmp(current->GetName(), name) > 0)
			current = current->GetLeft();
		else
			current = current->GetRight();
	}
	if (current == NULL) return false;

	if (strcmp(Root->GetName(), name) == 0) {
		if (current->GetLeft() == NULL && current->GetRight() == NULL) {
			delete current;
			Root = NULL;
		}
		else if (current->GetLeft() == NULL || current->GetRight() == NULL) {
			if (current->GetLeft() == NULL)
				leftright = current->GetRight();
			if (current->GetRight() == NULL)
				leftright = current->GetLeft();
			delete current;
			Root = leftright;
		}
		else if (current->GetLeft() != NULL && current->GetRight() != NULL) {
			parent = current;
			leftright = current->GetLeft(); // 왼쪽가서
			while (leftright->GetRight() != NULL) { // 최대가진 노드 찾기. parent는 최대노드의 부모노드
				parent = leftright;
				leftright = leftright->GetRight();
			}
			if (parent->GetRight() == leftright) // 서브트리의 노드가 존재
				parent->SetRight(leftright->GetLeft());
			else // 서브트리의 노드가 없음
				parent->SetLeft(leftright->GetLeft());
			Root->SetName(leftright->GetName());
			Root->SetDisease(leftright->GetDisease());
			delete parent, leftright;
		}
	}
	else if (current->GetLeft() == NULL && current->GetRight() == NULL) {
		if (parent->GetLeft() == current)
			parent->SetLeft(NULL);
		else if (parent->GetRight() == current)
			parent->SetRight(NULL);
		delete current;
	}
	else if (current->GetLeft() == NULL || current->GetRight() == NULL) {
		if (current->GetLeft() == NULL)
			leftright = current->GetRight();
		else if (current->GetRight() == NULL)
			leftright = current->GetLeft();
		if (parent->GetLeft() == current)
			parent->SetLeft(leftright);
		else if (parent->GetRight() == current)
			parent->SetRight(leftright);
		delete current;
	}
	else if (current->GetLeft() != NULL && current->GetRight() != NULL) {
		parent = current;
		leftright = current->GetLeft(); // 왼쪽가서
		while (leftright->GetRight() != NULL) { // 최대가진 노드 찾기. parent는 최대노드의 부모노드
			parent = leftright;
			leftright = leftright->GetRight();
		}
		if (parent->GetRight() == leftright) // 서브트리의 노드가 존재
			parent->SetRight(leftright->GetLeft());
		else // 서브트리의 노드가 없음
			parent->SetLeft(leftright->GetLeft());
		current->SetName(leftright->GetName());
		current->SetDisease(leftright->GetDisease());
		delete parent, leftright;
	}
	return true;
}

void PatientBST::Print_PRE(PatientBSTNode * node)
{
    //PRINT B PRE
	ofstream flog;
	flog.open("log.txt", ios::app);
	if (node != NULL) {
		flog << node->GetName() << "/" << node->GetDisease() << endl;
		Print_PRE(node->GetLeft());
		Print_PRE(node->GetRight());
	}
}

void PatientBST::Print_IN(PatientBSTNode * node)
{
    //PRINT B IN
	ofstream flog;
	flog.open("log.txt", ios::app);
	if (node != NULL) {
		Print_IN(node->GetLeft());
		flog << node->GetName() << "/" << node->GetDisease() << endl;
		Print_IN(node->GetRight());
	}
}

void PatientBST::Print_POST(PatientBSTNode * node)
{
    //PRINT B POST
	ofstream flog;
	flog.open("log.txt", ios::app);
	if (node != NULL) {
		Print_POST(node->GetLeft());
		Print_POST(node->GetRight());
		flog << node->GetName() << "/" << node->GetDisease() << endl;
	}
}

void PatientBST::Print_LEVEL()
{
    //PRINT B LEVEL
	ofstream flog;
	flog.open("log.txt", ios::app);
	queue <PatientBSTNode* > lv_queue;
	lv_queue.push(Root);
	lv_queue.push(NULL);
	PatientBSTNode* current = new PatientBSTNode();
	while (lv_queue.size() > 1) {
		current = lv_queue.front();
		lv_queue.pop();
		if (current == NULL)
			lv_queue.push(NULL);
		else {
			if (current->GetLeft())
				lv_queue.push(current->GetLeft());
			if (current->GetRight())
				lv_queue.push(current->GetRight());
			flog << current->GetName() << "/" << current->GetDisease() << endl;
		}
	}
}
