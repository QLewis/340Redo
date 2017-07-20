#include "stack.h"

stack::stack()
{
	top = -1; //initalize the stack to be empty
}

void stack::push(char insert)
{
	if (top < maximum)
	{
		top++;
		array[top] = insert;
	}
	/*else
	{
		cout << "The stack is full\n" << endl;
	}*/
}

char stack::pop()
{
	if (top == -1)
	{
		//cout << "The stack is empty\n" << endl;
		return NULL;
	}
	else
	{
		char first = array[top];
		array[top] = NULL;
		top--;
		return first;
	}
}

string stack::toString()
{
	string print = "";
	if (top == -1)
	{
		return NULL;
	}
	else
	{
		for (int i = 0; i < (top + 1); i++)
		{
			print += array[i];
		}
		return print;
	}
}

bool stack::isEmpty()
{
	if (top == -1)
	{
		//cout << "The stack is empty\n" << endl;
		return true;
	}
	else
	{
		return false;
	}
}
