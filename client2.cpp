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
	char valueToBinary;
};
struct ServerResponse
{
	char beginningProcess;
	char endingProcess;
	int EM[12];
};
void decodeMessage(ServerResponse &respond)
{
	vector<int> dm(12);
	int answer = 0;
	const vector<int> w1 { -1, 1, -1, 1 };
	const vector<int> w2 { -1, -1, 1, 1 };
	const vector<int> w3 { -1, 1, 1, -1 };
	vector<int> tempW;
	switch (respond.beginningProcess) //find out which process Walsh Code is needed to decode
	{								  //for current process
	case 1:
		tempW = w1; 
		break;

	case 2:
		tempW = w2;
		break;

	case 3:
		tempW = w3;
		break;
	}

	for (int i = 0; i < 12; i++)
	{
		dm[i] = w1[i % 4] * respond.EM[i];
	}
	cout << "Child " << respond.endingProcess << endl;
	cout << "Signal:" << respond.EM << endl;
	cout << "Code: ";
	for (int i = 0; i < 3; i++)
	{
		if (((dm[0 + 4 * i] + dm[1 + 4 * i] + dm[2 + 4 * i] + dm[3 + 4 * i]) / 4) == 1)
		{
			cout << 1 << " ";
			answer += pow(2, 2 - i);
		}
		else
		{
			cout << -1 << " ";
		}
	}
	cout << endl;
	cout << "Received Value = " << answer << endl;
}

int main(int argc, char* argv[])
{
	ServerRequest request;
	ServerResponse response;
	char incomingMessage[100];
	memset(incomingMessage, 0, 100);
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int status;
	int processNumber;
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
	while (cin >> request.endingProcess >> request.valueToEncode)
	{	
		serverAddress.sin_port = htons(port + processNumber);

		cout << "Child, " <<  << " sending value: " << valueToSend <<
			" " << "to child process " << destination << "." << endl;
			
		processNumber++;
	}
	//serverAddress.sin_port = htons(port + processNumber);
	for(int i = 0; i < 3; i++)
	{
		sleep(1);	
		pid_t pid; //fork section of the code
			
		if(pid == 0)
		{
			serverFD = socket(AF_INET, SOCK_STREAM, 0);
			if ((serverFD  < 0)
			{
				throw runtime_error("Socket failed");
			}
			
			if(connect(serverFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
			{
				throw runtime_error("Could not establish a connection");
			}
			if(i == 0)
			{
				write(serverFD, &request, sizeof(ServerRequest));
			}
			
			break;
		}
	}
	
	close(serverFD);

	return 0;
}
