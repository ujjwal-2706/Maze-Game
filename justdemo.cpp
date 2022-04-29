#include <iostream>
#include <string>
using namespace std;

void get(string val)
{
	cout << val;
}

int main()
{
	string val = "hello";
	val = val + to_string(34);
	cout << val << endl;
	cout << rand() << endl;
	cout << rand() << endl;
	return 0;
}