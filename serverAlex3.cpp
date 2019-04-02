// compile: g++ -std=c++11 -g -o server server.cpp
// Usage: ./executable_name port

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

using namespace std;

struct ServerRequest
{
	char beginningProcess;
	char endingProcess;
	char valueToEncode;
};
struct ServerResponse
{
	char beginningProcess;
	char endingProcess;
	char EM[12];
};

void encode(ServerRequest *req, ServerResponse &res)
{
	const vector<int> w1{-1, 1, -1, 1};
	const vector<int> w2{-1, -1, 1, 1};
	const vector<int> w3{-1, 1, 1, -1};
	int b1[3], b2[3], b3[3];
	int em1[12], em2[12], em3[12];
	for (int i = 0; i < 3; i++)
	{
		b1[i] = (req[0].valueToEncode >> i) & 1;
		b2[i] = (req[1].valueToEncode >> i) & 1;
		b3[i] = (req[2].valueToEncode >> i) & 1;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			em1[4 * i + j] = b1[i] * w1[j];
			em2[4 * i + j] = b2[i] * w2[j];
			em3[4 * i + j] = b3[i] * w3[j];
		}
	}
	for (int i = 0; i < 12; i++)
	{
		res.EM[i] = em1[i] + em2[i] + em3[i];
	}
}
int main(int argc, char *argv[])
{
	ServerRequest request[3];
	ServerResponse response;
	memset(response.EM, 0, 12 * sizeof(int));
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int serverFD;
	struct sockaddr_in serverAddress;
	struct hostent *serverInfo;

	port = atoi(argv[1]); //only connecting to one port

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port); //only connecting to one port

	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw runtime_error("Socket unavailable");
	}
	if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		throw runtime_error("Unable to bind the address to the file descriptor");
	}

	listen(serverFD, 3);
	cout << "Listening for clients..." << endl;
	cout.flush();

	// need to accept 3 times before reading
	int clientFD1, clientFD2, clientFD3;
	struct sockaddr_in clientAddress1, clientAddress2, clientAddress3;
	int clientAddressLength1 = sizeof(clientAddress1);
	int clientAddressLength2 = sizeof(clientAddress2);
	int clientAddressLength3 = sizeof(clientAddress3);
	clientFD1 = accept(serverFD, (sockaddr *)&clientAddress1, (socklen_t *)&clientAddressLength1);
	clientFD2 = accept(serverFD, (sockaddr *)&clientAddress2, (socklen_t *)&clientAddressLength2);
	clientFD3 = accept(serverFD, (sockaddr *)&clientAddress3, (socklen_t *)&clientAddressLength3);
	if (clientFD1 < 0)
	{
		throw runtime_error("Error accepting client 1 to the server");
	}
	if (clientFD2 < 0)
	{
		throw runtime_error("Error accepting client 2 to the server");
	}
	if (clientFD3 < 0)
	{
		throw runtime_error("Error accepting client 3 to the server");
	}
	cout << "Connected to clients." << endl;
	cout.flush();

	read(clientFD1, &request[0], sizeof(ServerRequest));
	read(clientFD2, &request[1], sizeof(ServerRequest));
	read(clientFD3, &request[2], sizeof(ServerRequest));
	cout << "Received messages from clients." << endl;
	cout.flush();

	cout << (int)request[0].beginningProcess << " "
		 << (int)request[0].endingProcess << " "
		 << (int)request[0].valueToEncode << endl;
	cout << (int)request[1].beginningProcess << " "
		 << (int)request[1].endingProcess << " "
		 << (int)request[1].valueToEncode << endl;
	cout << (int)request[2].beginningProcess << " "
		 << (int)request[2].endingProcess << " "
		 << (int)request[2].valueToEncode << endl;

	encode(request, response);
	cout << "Finished encoding the messages." << endl;
	for (int i = 0; i < 12; i++)
	{
		cout << (int)response.EM[i] << " ";
	}
	cout << endl;

	ServerResponse response1, response2, response3;
	for (int i = 0; i < 12; i++) {
		response1.EM[i] = response.EM[i];
		response2.EM[i] = response.EM[i];
		response3.EM[i] = response.EM[i];
	}

	response1.beginningProcess = request[0].beginningProcess;
	response1.endingProcess = request[0].endingProcess;
	write(clientFD1, &response1, sizeof(ServerResponse));
	sleep(1); //sleeps for one second to ensure data transferred properly in between each process

	response2.beginningProcess = request[1].beginningProcess;
	response2.endingProcess = request[1].endingProcess;
	write(clientFD2, &response2, sizeof(ServerResponse));
	sleep(1);

	response3.beginningProcess = request[2].beginningProcess;
	response3.endingProcess = request[2].endingProcess;
	write(clientFD3, &response3, sizeof(ServerResponse));
	sleep(1);

	cout << "Finished sending encoded messages to clients." << endl;
	cout.flush();

	close(clientFD1);
	close(clientFD2);
	close(clientFD3);

	close(serverFD);

	return 0;
}
