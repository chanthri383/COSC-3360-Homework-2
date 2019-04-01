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
	char valueToBinary;
};

struct ServerResponse
{
	char beginningProcess;
	char endingProcess;
	int EM[12];
};

void encode(ServerRequest *req, ServerResponse &res)
{
	const vector<int> w1{-1, 1, -1, 1};
	const vector<int> w2{-1, -1, 1, 1};
	const vector<int> w3{-1, 1, 1, -1};
	int b1[3];
	for (int i = 0; i < 3; i++)
	{
		b1[i] = (req[0].valueToBinary >> i) & 1;
	}
	int em1[12];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			em1[4 * i + j] = b1[i] * w1[j];
		}
	}

	int b2[3];
	for (int i = 0; i < 3; i++)
	{
		b2[i] = (req[1].valueToBinary >> i) & 1;
	}
	int em2[12];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			em2[4 * i + j] = b1[i] * w2[j];
		}
	}

	int b3[3];
	for (int i = 0; i < 3; i++)
	{
		b3[i] = (req[2].valueToBinary >> i) & 1;
	}
	int em3[12];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
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

	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw runtime_error("Socket unavailable");
	}

	strncpy(hostName, argv[1], 100);
	if ((serverInfo = gethostbyname(hostName)) == NULL)
	{
		throw runtime_error("Invalid host");
	}

	port = atoi(argv[2]); //only connecting to one port

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	bcopy((char *)serverInfo->h_addr,
		  (char *)&serverAddress.sin_addr.s_addr,
		  serverInfo->h_length);

	serverAddress.sin_port = htons(port); //only connecting to one port

	listen(serverFD, 3);

	// need to accept 3 times before reading
	int clientFD1, clientFD2, clientFD3;
	cout << "Listening for clients..." << endl;
	struct sockaddr_in clientAddress1, clientAddress2, clientAddress3;
	int clientAddressLength1 = sizeof(clientAddress1);
	int clientAddressLength2 = sizeof(clientAddress2);
	int clientAddressLength3 = sizeof(clientAddress3);
	if (clientFD1 = accept(serverFD, (sockaddr *)&clientAddress1, (socklen_t *)&clientAddressLength1) < 0)
	{
		throw runtime_error("Error accepting client 1 to the server");
	}
	if (clientFD2 = accept(serverFD, (sockaddr *)&clientAddress2, (socklen_t *)&clientAddressLength2) < 0)
	{
		throw runtime_error("Error accepting client 2 to the server");
	}
	if (clientFD3 = accept(serverFD, (sockaddr *)&clientAddress3, (socklen_t *)&clientAddressLength3) < 0)
	{
		throw runtime_error("Error accepting client 3 to the server");
	}

	read(clientFD1, &request[0], sizeof(ServerRequest));
	read(clientFD2, &request[1], sizeof(ServerRequest));
	read(clientFD3, &request[2], sizeof(ServerRequest));
	cout << "Received messages from clients." << endl;

	encode(request, response);
	cout << "Finished encoding the messages." << endl;

	response.beginningProcess = request[0].beginningProcess;
	response.endingProcess = request[0].endingProcess;
	write(clientFD1, &response, sizeof(ServerResponse));
	sleep(1); //sleeps for one second to ensure data transferred properly in between each process

	response.beginningProcess = request[1].beginningProcess;
	response.endingProcess = request[1].endingProcess;
	write(clientFD2, &response, sizeof(ServerResponse));
	sleep(1);

	response.beginningProcess = request[2].beginningProcess;
	response.endingProcess = request[2].endingProcess;
	write(clientFD3, &response, sizeof(ServerResponse));
	sleep(1);

	cout << "Finished sending encoded messages to clients." << endl;

	close(clientFD1);
	close(clientFD2);
	close(clientFD3);

	close(serverFD);

	return 0;
}
