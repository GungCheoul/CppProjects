#pragma once
#include <iostream>
using namespace std;
class Monster;

class Human
{
private:
	int index;
	Human* prev;
	Human* next;
public:
	int health;
	int power;
	int row;
	int column;
	Human()
	{
		row = column = health = power = 0;
		index = 0;
		next = prev = NULL;
	}
	void Action(Monster* monster);
	int getindex() { return index; }
	Human* getnext() { return next; }
	Human* getprev() { return prev; }
	void setindex(int idx) { index = idx; }
	void setnext(Human* snext) { next = snext; }
	void setprev(Human* sprev) { prev = sprev; }
};
class HDLL
{
private:
	int cidx;	
	Human* head;
	Human* tail;
public:
	HDLL();
	~HDLL();
	int getcidx();
	void Insert(int idx, Human* instance);
	void Delete(int index);
	void Print();
};
class Monster
{
private:
	char index;
	Monster* prev;
	Monster* next;
public:
	int health;
	int power;
	int row;
	int column;
	Monster()
	{
		row = column = 0;
		health = 20;
		power = 7;
		index = '0';
		next = prev = NULL;
	}
	Monster(char ch, int r, int c)
	{
		index = ch;
		row = r;
		column = c;
		health = 20;
		power = 7;
		next = prev = NULL;
	}
	void Action(Human* human);
	char getindex() { return index; }
	Monster* getnext() { return next; }
	Monster* getprev() { return prev; }
	void setindex(char idx) { index = idx; }
	void setnext(Monster* snext) { next = snext; }
	void setprev(Monster* sprev) { prev = sprev; }
};
class MDLL
{
private:
	char cidx;
	Monster* head;
	Monster* tail;
public:
	MDLL();
	~MDLL();
	char getcidx();
	void Insert(Monster* instance);
	void Delete(char index);
	void Print();
};
class PeasantArmy : public Human
{
public:
	PeasantArmy()
	{
		row = column = 0;
		health = 5;
		power = 5;
	}
	PeasantArmy(int r, int c)
	{
		row = r;
		column = c;
		health = 5;
		power = 5;
	}
	void Action(Monster* monster)
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
};
class SwordMaster : public Human
{
public:
	SwordMaster()
	{
		row = column = 0;
		health = 10;
		power = 10;
	}
	SwordMaster(int r, int c)
	{
		row = r;
		column = c;
		health = 10;
		power = 10;
	}
	void Action(Monster* monster)
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
};
class Archer : public Human
{
public:
	Archer()
	{
		row = column = 0;
		health = 7;
		power = 10;
	}
	Archer(int r, int c)
	{
		row = r;
		column = c;
		health = 7;
		power = 10;
	}
	void Action(Monster* monster)
	{
		if (monster->column == column + 1 || monster->column == column + 2) // attack
			monster->health = monster->health - power;
		else if (monster->column == column - 1 || monster->column == column - 2) // attack
			monster->health = monster->health - power;
		else if (monster->row == row + 1 || monster->row == row + 2) // attack
			monster->health = monster->health - power;
		else if (monster->row == row - 1 || monster->row == row - 2) // attack
			monster->health = monster->health - power;
		else if (monster->column == column - 1 && monster->row == row - 1) // attack
			monster->health = monster->health - power;
		else if (monster->column == column - 1 && monster->row == row + 1) // attack
			monster->health = monster->health - power;
		else if (monster->column == column + 1 && monster->row == row - 1) // attack
			monster->health = monster->health - power;
		else if (monster->column == column + 1 && monster->row == row + 1) // attack
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
};
class Warlock : public Human
{
private:
	bool mana;
public:
	Warlock()
	{
		row = column = 0;
		health = 3;
		power = 10;
		mana = true;
	}
	Warlock(int r, int c)
	{
		row = r;
		column = c;
		health = 3;
		power = 10;
		mana = true;
	}
	void Action(Monster* monster)
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
};