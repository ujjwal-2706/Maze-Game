#include <iostream>
#include <string>
#include <vector>
#include "server.h"
#include "client.h"
using namespace std;

int main(int argc, char** argv)
{
	char* message1 = "my position is 3,4";
	char* message2 = "my yulu is broken";
	string val = argv[1];
	bool server  = false;
	bool client = false;
	// cout << argv[1] << endl;
	if(val.compare("server") == 0) {server = true; makeServerSide();} 
	if(val.compare("client") == 0) {client = true; makeClientSide();}

	while(true)
	{
		if(server)
		{
			
			string val = readFromClient();
			cout << val;
			sendToClient(message1);
		}
		else
		{
			
			sendToServer(message2);
			string val2 = readFromServer();
			cout << val2;
		}
	}
	return 0;
}