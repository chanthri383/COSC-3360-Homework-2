// compile: g++ -std=c++11 -g -o client client.cpp
// Usage: ./executable_name hostname port < in.txt

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cmath>
#include <sys/wait.h>

using namespace std;

struct ServerRequest
{
	int beginningProcess;
	int endingProcess;
	int valueToEncode;
};
struct ServerResponse
{
	int beginningProcess;
	int endingProcess;
	int EM[12];
};
void decodeMessage(ServerResponse &respond)
{
	vector<int> dm(12);
	int answer = 0;
	int stored4bits;
	const vector<int> w1{-1, 1, -1, 1};
	const vector<int> w2{-1, -1, 1, 1};
	const vector<int> w3{-1, 1, 1, -1};
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

	cout << "Child " << respond.endingProcess << endl;
	cout << "Signal: ";
	for (int i = 0; i < 12; i++)
	{
		dm[i] = w1[i % 4] * respond.EM[i];
		cout << respond.EM[i] << " ";
	}
	cout << endl;
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

int main(int argc, char *argv[])
{
	ServerRequest request;
	ServerResponse response;
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int serverFD;
	struct hostent *serverInfo;
	struct sockaddr_in serverAddress;

	strncpy(hostName, argv[1], 100);
	if ((serverInfo = gethostbyname(hostName)) == NULL)
	{
		throw runtime_error("Invalid host");
	}
	port = atoi(argv[2]);
	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	bcopy((char *)serverInfo->h_addr,
		  (char *)&serverAddress.sin_addr.s_addr,
		  serverInfo->h_length);
	serverAddress.sin_port = htons(port); //we do not have to create multiple ports just one

	int processNumber = 1;
	pid_t pid;

	for (int i = 0; i < 3; i++)
	{
		cin >> request.endingProcess >> request.valueToEncode;
		request.beginningProcess = processNumber;
		cout << "Child, " << processNumber << " sending value: " << request.valueToEncode << " "
			 << "to child process " << request.endingProcess << "." << endl;

		cout << request.beginningProcess << " " << request.endingProcess << " " << request.valueToEncode << endl;
		processNumber++; // child 1, 2 and 3 affected on cout when incrementing this value

		if ( (pid = fork()) == 0 )
		{
			break;
		}
	}

	if (pid != 0) {
		int status = 0;
		for (int i = 0; i < 3; i++)
		{
			wait(&status);
		}
		return 0;
	}

	// from now on, this is child process code
	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw runtime_error("Socket unavailable");
	}

	if (connect(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		// this is thrown
		throw runtime_error("Could not establish a connection");
	}

	write(serverFD, &request, sizeof(ServerRequest)); //assume we write the value of binary value and which process to send back

	read(serverFD, &response, sizeof(ServerResponse));

	decodeMessage(response); // also prints out any remaining necessary output

	return 0;
}
