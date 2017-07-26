#include <iostream>
#include <string>

using namespace std;

struct symbolTable
{
	symbolTableItem* item;
	symbolTable* next;
	symbolTable* previous;
};

struct symbolTableItem
{
	char* name;
	char* scope;
	int permission; //1 for public, 0 for private
};

class SymbolTable
{
	private:
		struct symbolTable* head;
	public:
		string currentScope;
		SymbolTable();
		addItem(char* name, char* scope, int permission);
		symbolTableItem searchItem(char* name);
};


//Constructor
SymbolTable::SymbolTable()
{
	head = NULL;
}

SymbolTable::addItem(char* name, char* scope, int permission)
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

symbolTableItem SymbolTable::searchItem(char* searchName)
{
	SymbolTable* traverse = head;

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