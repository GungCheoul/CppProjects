#include "class.h"
#include <iostream>
using namespace std;

int main()
{
	HDLL* Humans = new HDLL();
	MDLL* Monsters = new MDLL();

	PeasantArmy* PA = new PeasantArmy(0, 0);
	SwordMaster* SM1 = new SwordMaster(1, 0);
	SwordMaster* SM2 = new SwordMaster(2, 0);
	Archer* Arch = new Archer(3, 0);
	Warlock* WL = new Warlock(4, 0);
	Monster* M1 = new Monster('A', 0, 4);
	Monster* M2 = new Monster('B', 1, 4);
	Monster* M3 = new Monster('C', 2, 4);
	Monster* M4 = new Monster('D', 3, 4);
	Monster* M5 = new Monster('E', 4, 4);
	
	Humans->Insert(1, PA);
	Humans->Insert(2, SM1);
	Humans->Insert(3, SM2);
	Humans->Insert(4, Arch);
	Humans->Insert(5, WL);
	Monsters->Insert(M1);
	Monsters->Insert(M2);
	Monsters->Insert(M3);
	Monsters->Insert(M4);
	Monsters->Insert(M5);
	
	int r = 1;
	while (1)
	{
		if (Humans->getcidx() == 0 || Monsters->getcidx() == 0)
			break;
		PA->Action(M1);
		SM1->Action(M2);
		SM2->Action(M3);
		Arch->Action(M4);
		WL->Action(M5);
		M1->Action(PA);
		M2->Action(SM1);
		M3->Action(SM2);
		M4->Action(Arch);
		M5->Action(WL);

		if (PA->health == 0 || PA->health < 0)
			Humans->Delete(1);
		if (SM1->health == 0 || SM1->health < 0)
			Humans->Delete(2);
		if (SM2->health == 0 || SM2->health < 0)
			Humans->Delete(3);
		if (Arch->health == 0 || Arch->health < 0)
			Humans->Delete(4);
		if (WL->health == 0 || WL->health < 0)
			Humans->Delete(5);
		if (M1->health == 0 || M1->health < 0)
			Monsters->Delete('A');
		if (M2->health == 0 || M2->health < 0)
			Monsters->Delete('B');
		if (M3->health == 0 || M3->health < 0)
			Monsters->Delete('C');
		if (M4->health == 0 || M4->health < 0)
			Monsters->Delete('D');
		if (M5->health == 0 || M5->health < 0)
			Monsters->Delete('E');

		cout << "#### Result of round " << r++ << endl;
		cout << "<Human>" << endl;
		Humans->Print();
		cout << "<Monster>" << endl;
		Monsters->Print();
	}

	delete Humans;
	delete Monsters;
	return 0;
}