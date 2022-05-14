#include "LocationBST.h"

LocationBST::LocationBST()
{
	Root = NULL;// new LocationNode();
}

LocationBST::~LocationBST()
{
	Root = NULL;// new LocationNode();
}

LocationNode * LocationBST::GetRoot()
{
	return Root;
}

void LocationBST::Insert_Location(LocationNode * node)
{
    //Build once in Constructor
	if (!Root)
		Root = node;
		//Root->SetLoc(node->GetLoc());
	else {
		LocationNode* current = new LocationNode();
		LocationNode* parent = new LocationNode();
		current = Root;
		while (current != NULL) {
			parent = current;
			if (strcmp(current->GetLoc(), node->GetLoc()) > 0)
				current = current->GetLeft();
			else
				current = current->GetRight();
		}
		if (strcmp(parent->GetLoc(), node->GetLoc()) > 0)
			parent->SetLeft(node);
		else
			parent->SetRight(node);
	}
}

bool LocationBST::Insert_Patient(PatientNode * node)
{
    //QPOP
	LocationNode* current = new LocationNode();
	current = Root;
	while (current != NULL) {
		if (strcmp(current->GetLoc(), node->GetLoc()) == 0) {
			current->GetBST()->Insert(node);
			break;
		}
		if (strcmp(current->GetLoc(), node->GetLoc()) > 0)
			current = current->GetLeft();
		else
			current = current->GetRight();
	}
	return true;
}

bool LocationBST::Search(char * name)
{
    //SEARCH
	bool searched = false;
	LocationNode *current = new LocationNode();
	stack <LocationNode*> s_stack;
	s_stack.push(Root);
	while (!s_stack.empty()) {
		current = s_stack.top();
		if (current->GetBST() != NULL && current->GetBST()->Search(name) == true) {
			searched = true;
			break;
		}
		s_stack.pop();
		if (current->GetRight())
			s_stack.push(current->GetRight());
		if (current->GetLeft())
			s_stack.push(current->GetLeft());
	}
	return searched;
}

char * LocationBST::Delete(char * name)
{
    //BPOP
	LocationNode *current = new LocationNode();
	stack <LocationNode*> d_stack;
	d_stack.push(Root);
	while (!d_stack.empty()) {
		current = d_stack.top();
		if (current->GetBST()->GetRoot() && current->GetBST()->Delete(name) == true)
			return current->GetLoc();
		d_stack.pop();
		if (current->GetRight())
			d_stack.push(current->GetRight());
		if (current->GetLeft())
			d_stack.push(current->GetLeft());
	}
	return NULL;
}

void LocationBST::Print_PRE()
{
    //PRINT B PRE
	LocationNode *current = new LocationNode();
	stack <LocationNode*> pre_stack;
	pre_stack.push(Root);
	while (!pre_stack.empty()) {
		current = pre_stack.top();
		if (current->GetBST()->GetRoot())
			current->GetBST()->Print_PRE(current->GetBST()->GetRoot());
		pre_stack.pop();
		if (current->GetRight())
			pre_stack.push(current->GetRight());
		if (current->GetLeft())
			pre_stack.push(current->GetLeft());
	}
}

void LocationBST::Print_IN()
{
    //PRINT B IN
	LocationNode *current = new LocationNode();
	current = Root;
	stack <LocationNode*> in_stack;
	while (current != NULL || !in_stack.empty()) {
		while (current != NULL) {
			in_stack.push(current);
			current = current->GetLeft();
		}
		current = in_stack.top();
		in_stack.pop();
		if (current->GetBST()->GetRoot())
			current->GetBST()->Print_IN(current->GetBST()->GetRoot());
		current = current->GetRight();
	}
}

void LocationBST::Print_POST()
{
    //PRINT B POST
	LocationNode *current = new LocationNode();
	stack <LocationNode*> post_stack1, post_stack2;
	post_stack1.push(Root);
	while (!post_stack1.empty()) {
		current = post_stack1.top();
		post_stack1.pop();
		post_stack2.push(current);
		if (current->GetLeft())
			post_stack1.push(current->GetLeft());
		if (current->GetRight())
			post_stack1.push(current->GetRight());
	}
	while (!post_stack2.empty()) {
		current = post_stack2.top();
		post_stack2.pop();
		if (current->GetBST()->GetRoot())
			current->GetBST()->Print_POST(current->GetBST()->GetRoot());
	}
}

void LocationBST::Print_LEVEL()
{
    //PRINT B LEVEL
	queue <LocationNode* > lv_queue;
	lv_queue.push(Root);
	lv_queue.push(NULL);
	LocationNode* current = new LocationNode();
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

			if (current->GetBST()->GetRoot())
				current->GetBST()->Print_LEVEL();
		}
	}
}
