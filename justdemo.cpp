#include <iostream>
#include <string>
#include <vector>
using namespace std;

void get(string val)
{
	cout << val;
}

int main()
{
	vector<vector<int>> result;
	vector<int> temp1;
	vector<int> temp2;
	result.push_back(temp1);
	result.push_back(temp2);
	vector<int> temp3;
	temp3.push_back(23);
	result.push_back(temp3);
	cout << result[2][0];
}