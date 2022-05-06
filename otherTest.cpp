#include <iostream>
#include <string>
#include <vector>
using namespace std;

string paramToData(vector<int> playerParam)
{
	// 0 -> coins
	// 1 -> health
	// 2 -> stamina
	// 3 -> motivation
	// 4 -> mapX
	// 5 -> mapY
	// 6 -> x
	// 7 -> y
	// 8 -> onYulu
	string value = "" + to_string(playerParam[0]) + " " + to_string(playerParam[1]) + " ";
	value += to_string(playerParam[2]) + " " + to_string(playerParam[3]) + " " + to_string(playerParam[4]);
	value += " " + to_string(playerParam[5]) + " " + to_string(playerParam[6]) + " " + to_string(playerParam[7]);
	value += " " + to_string(playerParam[8]);
	string result = value.c_str();
	return result;
}

vector<int> dataToParam(string value)
{
	vector<int> result;
	if(value.compare("") == 0)
	{
		return result;
	}
	else
	{
		int index = 0;
		int prev = 0;
		while(index < value.size())
		{
			if((value.substr(index,1)).compare(" ") == 0)
			{
				int num = stoi(value.substr(prev,index - prev));
				result.push_back(num);
				index ++;
				prev = index;
			}
			else
			{
				index ++;
			}

		}
        int num = stoi(value.substr(prev,index - prev));
        result.push_back(num);
		return result;
	}
}
int main()
{
    string value = "my name is";
    const char* val = value.c_str();
    vector<int> value1;
    value1 = dataToParam("10 23 34 45 9123 2 13 45 6");
    string value2 = paramToData(value1);
    // for(int i =0; i < value1.size(); i++)
    // {
    //     cout << value1[i] << " ";
    // }
    // cout << value1.size() <<  endl;
    const char* get = value2.c_str();
    cout << get;
    return 0;
}