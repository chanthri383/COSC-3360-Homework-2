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
#include <arpa/inet.h>

using namespace std;

struct ServerRequest
{
	int beginningProcess;
	int middleProcess;
	int endingProcess;
	int valueToBinary;
};

struct ServerResponse
{
	char beginningProcess;
	char endingProcess;
	int EM[12];
};

void encode(ServerRequest *req, ServerResponse &res)
{
	const vector<int> w1{ -1, 1, -1, 1 };
	const vector<int> w2{ -1, -1, 1, 1 };
	const vector<int> w3{ -1, 1, 1, -1 };
	int b1[3];
	for (int i = 0; i < 3; i++)
	{
		//b1[i] = (req[0].valueToBinary >> i) & 1;33
		
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
}
int main(int argc, char* argv[])
{
	ServerRequest request[3];
	ServerResponse response;
	memset(response.EM, 0, 12 * sizeof(int));
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int serverFD;

	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw runtime_error("Socket failure");
	}

	port = atoi(argv[1]);
	int port1, port2, port3;
	port1 = port + 1;
	port2 = port + 2;
	port3 = port + 3;

	struct sockaddr_in serverAddress;

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port1);

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port2);

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port3);


	serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(serverFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		throw runtime_error("Unable to bind the address to the file descriptor");
	}

	listen(serverFD, 3); 

	cout << "listening for clients..." << endl;

	int clientFD1, clientFD2, clientFD3;
	struct sockaddr_in clientAddress1, clientAddress2, clientAddress3;
	int clientAddressLength1 = sizeof(clientAddress1);
	int clientAddressLength2 = sizeof(clientAddress2);
	int clientAddressLength3 = sizeof(clientAddress3);
	clientFD1 = accept(serverFD, (sockaddr *)&clientAddress2, (socklen_t *)&clientAddressLength2);
	clientFD2 = accept(serverFD, (sockaddr *)&clientAddress2, (socklen_t *)&clientAddressLength2);
	clientFD3 = accept(serverFD, (sockaddr *)&clientAddress3, (socklen_t *)&clientAddressLength3);

	read(clientFD, &(request[0]), sizeof(ServerRequest));
	read(clientFD, &(request[1]), sizeof(ServerRequest));
	read(clientFD, &(request[2]), sizeof(ServerRequest));

	encode(request, response); //change

	response.beginningProcess = request[0].beginningProcess;
	response.endingProcess = request[0].endingProcess;
	write(clientFD1, &response, sizeof(ServerResponse));
	sleep(1);

	response.beginningProcess = request[1].beginningProcess;
	response.endingProcess = request[1].endingProcess;
	write(clientFD2, &response, sizeof(ServerResponse));
	sleep(1);

	response.beginningProcess = request[2].beginningProcess;
	response.endingProcess = request[2].endingProcess;
	write(clientFD3, &response, sizeof(ServerResponse));
	sleep(1);

	close(clientFD1);
	close(clientFD2);
	close(clientFD3);
	close(serverFD);

	return 0;
}
