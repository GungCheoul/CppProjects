#include "class.h"
#include <iostream>
using namespace std;

MDLL::MDLL()
{
	head = new Monster();
	tail = head;
	cidx = '0';
}
MDLL::~MDLL()
{
	Monster* temp = head->getnext();
	delete head;
	while (temp != NULL)
	{
		head = temp;
		temp = temp->getnext();
		delete head;
	}
}
char MDLL::getcidx()
{
	return cidx;
}
void MDLL::Insert(Monster* instance)
{
	Monster* newnode = instance;
	tail->setnext(newnode);
	newnode->setprev(tail);
	tail = newnode;
	cidx++;
}
void MDLL::Delete(char didx)
{
	Monster* pretmp = head;
	Monster* temp = pretmp->getnext();

	if (head->getnext() == NULL)
		cout << "No node" << endl;
	for (int i = 0; i < cidx; i++)
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
void MDLL::Print()
{
	Monster* temp = head->getnext();

	while (temp != NULL)
	{
		cout << temp->getindex() << endl;
		temp = temp->getnext();
	}
}
void Monster::Action(Human* human)
{
	if (human->column == column - 1 || human->column == column + 1) // attack
		human->health = human->health - power;
	else if (human->row == row - 1 || human->row == row + 1) // attack
		human->health = human->health - power;
	else // move
	{
		if (column < human->column)
			column + 1;
		else if (row > human->row)
			row - 1;
		else if (row < human->row)
			row + 1;
	}
}