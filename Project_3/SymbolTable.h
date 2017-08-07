#include <iostream>
#include <string>

using namespace std;

struct symbolTableItem
{
	string name;
	string type;
};

struct symbolTable
{
	symbolTableItem* item;
	symbolTable* next;
	symbolTable* previous;
};

class SymbolTable
{
	private:
		struct SymbolTable* head;
	public:
		SymbolTable();
		void addItem(string name, string type);
		void editType(string type);
		string searchItem(string name);
};

//Constructor
SymbolTable::SymbolTable()
{
	head = NULL;
}

void addItem(string name, string type)
{
	//Create new node for symbol table and create a new symbol table item
	symbolTable* symTab = new symbolTable();
	symbolTableItem* newItem = new symbolTableItem();

	newItem->name = name;
	newItem->type = type;

	if (head == NULL) //list is empty
	{
		head = symTab;
		symTab->item = newItem;
		symTab->next = NULL;
		symTab->previous = NULL;
	}
	else //add to end
	{
		symbolTable* traverser = head;

		while (traverser = traverser->next != NULL)
		{
			traverser = traverser->next;
		}

		traverser->next = symTab;
		symTab->item = newItem;
		symTab->next = NULL;
		symTab->previous = traverser;
	}
}

void editType(string type)
{
	symbolTable* traverser = head;
	
	while (traverser != null)
	{
		if ((traverser->item->name == name) && (traverser->item->type == "?"))
		{
			traverser->item->type = type;
		}

		traverser = traverser->next;
	}
}

string SymbolTable::searchItem(string searchName)
{
	cerr << "SymbolTable.h -- INSIDE SEARCHITEM\n" << endl;

	symbolTable* traverser = head;

	string tmpString;

	if (head == NULL) //the list is empty
	{
		return "?";
	}

	while (traverser->next != NULL)
	{
		traverser = traverser->next;
	}

	//search, starting with the most recent node added
	do
	{
		if (traverser->item->name == searchName)
		{
			cerr << "found " << traverser->item->name << ", " traverser->item->type << endl << endl;
			return traverser->item->type;
		}
	}
}