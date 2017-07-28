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
		void removeScope(string scopeName);
		string searchItem(string name);
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

void SymbolTable::removeScope(string scopeName)
{
	symbolTable* traverse = head;
	symbolTable* grab = head;

	while (traverse != NULL)
	{
		if (traverse->item->scope == scopeName)
		{
			grab =  traverse;

			if (traverse != head) //not at the head
			{
				traverse = traverse->previous;
				traverse->next = grab->next;
				grab->next->previous = traverse;
				delete grab;
			}
			else
			{
				delete grab;
			}
		}
		traverse = traverse->next;
	}
}

string SymbolTable::searchItem(string searchName)
{
	symbolTable* traverse = head;
	string tmpString;

	while (traverse->next != NULL) //go to the end of the linked list
	{
		traverse = traverse->next; 
	}

	//search, starting with the most recent node added
	while (traverse->previous != NULL)
	{
		if (traverse->item->name == searchName) //If variable with name is found
		{
			if (traverse->item->scope == currentScope) //check that scope matches current scope
			{
				if (currentScope == "::")
				{
					tmpString = "::";
				}
				else
				{
					tmpString = currentScope + ".";
				}
				return tmpString;
			}
			else
			{
				if (traverse->item->permission == 1) //check that non-matching scope is public
				{
					if (traverse->item->scope == "::")
					{
						tmpString = "::";
					}
					else
					{
						tmpString = traverse->item->scope + ".";
					}
					return tmpString;
				}
			}
		}
		traverse = traverse->previous;
	}
	return "?.";

}