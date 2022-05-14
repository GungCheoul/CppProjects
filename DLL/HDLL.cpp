#include "class.h"
#include <iostream>
using namespace std;

HDLL::HDLL()
{
	head = new Human();
	tail = head;
	cidx = 0;
}
HDLL::~HDLL()
{
	Human* temp = head->getnext();
	delete head;
	while (temp != NULL)
	{
		head = temp;
		temp = temp->getnext();
		delete head;
	}
}
int HDLL::getcidx()
{
	return cidx;
}
void HDLL::Insert(int idx, Human* instance)
{
	Human* newnode = instance;
	newnode->setindex(idx);
	tail->setnext(newnode);
	newnode->setprev(tail);
	tail = newnode;
	cidx++;
}
void HDLL::Delete(int didx)
{
	Human* pretmp = head;
	Human* temp = pretmp->getnext();

	while(temp!=NULL)
	{
		if (temp->getindex() == didx)
		{
			pretmp->setnext(temp->getnext());
			temp->getnext()->setprev(pretmp);
			if (temp == tail)
				tail = pretmp;
			delete temp;
			cidx--;
			break;
		}
		pretmp = temp;
		temp = temp->getnext();
	}
}
void HDLL::Print()
{
	Human* temp = head->getnext();

	while (temp != NULL)
	{
		cout << temp->getindex() << endl;
		temp = temp->getnext();
	}
}
void Human::Action(Monster* monster)
{
	if (monster->column == column - 1 || monster->column == column + 1) // attack
		monster->health = monster->health - power;
	else if (monster->row == row - 1 || monster->row == row + 1) // attack
		monster->health = monster->health - power;
	else // move
	{
		if (column < monster->column)
			column + 1;
		else if (row > monster->row)
			row - 1;
		else if (row < monster->row)
			row + 1;
	}
}