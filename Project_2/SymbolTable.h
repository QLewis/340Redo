#include <iostream>
#include <string>

using namespace std;

struct symbolTableItem
{
	string name;
	string scope;
	int permission; //1 for public, 0 for private
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
		struct symbolTable* head;
	public:
		string currentScope;
		int currentPermission;
		SymbolTable();
		void addItem(string name, string scope, int permission);
		symbolTableItem* searchItem(string name);
};


//Constructor
SymbolTable::SymbolTable()
{
	head = NULL;
}

void SymbolTable::addItem(string name, string scope, int permission)
{
	//Create new node for symbol table and create a new symbol table item
	symbolTable* symTab = new symbolTable();
	symbolTableItem* newItem = new symbolTableItem();

	newItem->name = name;
	newItem->scope = scope;
	newItem->permission = permission;

	if (head == NULL) //list is empty
	{
		head = symTab;
		symTab->item = newItem;
		symTab->next = NULL;
		symTab->previous = NULL;

	}
	else //add to end
	{
		symbolTable* traverse = head;
		while (traverse->next != NULL)
		{
			traverse = traverse->next;
		}
		traverse->next = symTab;
		symTab->item = newItem;
		symTab->next = NULL;
		symTab->previous = traverse;
	}
}

symbolTableItem* SymbolTable::searchItem(string searchName)
{
	symbolTable* traverse = head;

	while (traverse->next != NULL) //go to the end of the linked list
	{
		traverse = traverse->next;
	}

	//search, started with the most recent node added
	while (traverse->previous != NULL)
	{
		if (traverse->item->name == searchName)
		{
			return traverse->item;
		}
		traverse = traverse->previous;
	}

	return NULL; //if nothing is found
}