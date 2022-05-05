#include <iostream>
#include <string>
#include <vector>
using namespace std;

void get(string val)
{
	cout << val;
}

class Coord
{
	public:
		int x,y;
};

void func(Coord val)
{
	val.x = 10;
};
Coord val;
int main()
{
	
	
	return 0;
}