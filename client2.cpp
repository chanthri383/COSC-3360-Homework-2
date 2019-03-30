#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cmath>

using namespace std;

struct ServerRequest
{
	char beginningProcess;
	char endingProcess;
	int valueToBinary;
};

struct ServerResponse
{
	char beginningProcess;
	char endingProcess;
	int EM[12];
};

int decodeMessage(ServerResponse &respond) 
{
	vector<int> dm(12);
	int answer = 0;
	const vector<int> w1 { -1, 1, -1, 1 };
	const vector<int> w2 { -1, -1, 1, 1 };
	const vector<int> w3 { -1, 1, 1, -1 };
	vector<int> tempW;
	for (int i = 0; i < 12; i++)
	{
		dm[i] = w1[i % 4] * respond.EM[i];
	}
}

int main(int argc, char* argv[])
{
	ServerRequest request;
	ServerResponse response[3];
	char incomingMessage[100];
	memset(incomingMessage, 0, 100);
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int status; //not sure needed for wait
	int processNumber = 0;
	int serverFD;
	struct hostent* serverInfo;
	struct sockaddr_in serverAddress;
	strncpy(hostName, argv[1], 100);
	int clientFD1, clientFD2, clientFD3;
	if ((serverInfo = gethostbyname(hostName)) == NULL)
	{
		throw runtime_error("Cannot get host name");
	}
	
	port = atoi(argv[2]); 
	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	bcopy((char *)serverInfo->h_addr,
		(char *)&serverAddress.sin_addr.s_addr,
		serverInfo->h_length);
	while (cin >> request.process >> request.valueToEncode)
	{
		cout << "Child, " <<  << " sending value: " << valueToSend <<
			" " << "to child process " << destination << "." << endl;

	}
	for(int i = 0; i < 3; i++)
	{
		serverAddress.sin_port = htons(port);
		sleep(1);	
		pid_t pid; //fork section of the code
			
		if(pid == 0)
		{	
			serverAddress.sin_port = htons(port + processNumber);
			serverFD = socket(AF_INET, SOCK_STREAM, 0);
			if ((serverFD  < 0)
			{
				throw runtime_error("Socket failed");
			}
			
			if(connect(serverFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
			{
				throw runtime_error("Could not establish a connection");
			}
				write(serverFD, &request.beginningProcess, sizeof(ServerRequest));
			    	sleep(1);
			    	write(serverFD, &request.endingProcess, sizeof(ServerRequest));
			    	sleep(1);
			    	write(serverFD, &request.valueToBinary, sizeof(ServerRequest));
			    	sleep(1);
			    	read(serverFD, &response.beginningProcess, sizeof(ServerResponse));
			    	read(serverFD, &response.endingProcess, sizeof(ServerResponse));
			    	read(serverFD, &response.EM, sizeof(ServerResponse));
			    	
				break;
		}
	}
	
	close(serverFD);

	return 0;
}
