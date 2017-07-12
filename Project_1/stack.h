#ifndef __STACK__H__
#define __STACK__H__

#include <iostream>
using namespace std;

const int maximum = 100;

class stack
{
	private:
		int array[maximum];
		int top;
	public:
		stack();
		void push(char insert);
		char pop();
		bool isEmpty();
		
}
#endif // __STACK__H__
