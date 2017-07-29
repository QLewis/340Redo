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
	cerr << "SymbolTable.h -- inside removeScope\n" << endl;
	symbolTable* traverse = head;
	symbolTable* grab = head;

	while (traverse != NULL)
	{
		if (traverse->item->scope == scopeName)
		{
			if (traverse == head) //beginning of linked list
			{
				grab = traverse;
				head = head->next;
				traverse = head;
				delete grab;
			}
			else if (traverse->next == NULL) //end of linked list
			{
				grab = traverse;
				traverse = traverse->next;
				delete grab;
			}
			else//Situation 1
			{
				grab = traverse;
				traverse->previous->next = traverse->next;
				traverse->next->previous = traverse->previous;
				traverse = traverse->next;
				delete grab;
			}
		}
		else
		{
			traverse = traverse->next;
		}
	}
}

string SymbolTable::searchItem(string searchName)
{
	cerr << "SymbolTable.h -- INSIDE SEARCHITEM\n" << endl;
	symbolTable* traverse = head;
	string tmpString;

	if (head == NULL) //the list is empty
	{
		return "?.";
	}

	while (traverse->next != NULL) //go to the end of the linked list
	{
		traverse = traverse->next; 
	}

	//search, starting with the most recent node added
	do
	{
		if (traverse->item->name == searchName)
		{
			cerr << "found " << traverse->item->name << ", " << traverse->item->scope << ", " << traverse->item->permission << endl << endl;
			if (traverse->item->scope == currentScope)
			{
				cerr << "The scope of " << traverse->item->name << " (" << traverse->item->scope << ") matches " << currentScope << endl << endl;

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
				if (traverse->item->permission == 1)
				{
					cerr << "The scope of " << traverse->item->name << " (" << traverse->item->scope << ")  does not match " << currentScope << " but it is a public variable\n"<< endl;
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

	} while (traverse != NULL);
	return "?.";

}