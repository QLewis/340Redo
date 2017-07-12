#include "stack.h"

stack::Stack()
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
	else
	{
		cout << "The stack is full\n" << endl;
	}
}

char stack::pop()
{
	if (top == -1)
	{
		cout << "The stack is empty\n" << endl;
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

bool stack::isEmpty()
{
	if (top == -1)
	{
		cout << "The stack is empty\n" << endl;
		return true;
	}
	else
	{
		return false;
	}
}
