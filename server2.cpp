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
	int endingProcess;
	int valueToBinary;
};

struct ServerResponse
{
	int beginningProcess;
	int endingProcess;
	int EM[12];
};

string encode(ServerRequest &request.beginningProcess, ServerRequest &valueToBinary)
{
	int w1[4] = { -1, 1, -1, 1 };
	int w2[4] = { -1, -1, 1, 1 };
	int w3[4] = { -1, 1, 1, -1 };
	int binary[3];
	int em1[12];
	
	for (int i = 0; i < 3; i++)
	{
		binary[i] = (request.valueToBinary >> i) & 1;
	}
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
int main(int argc, char* argv[])
{
	ServerRequest request[3];
	int clientFD1, clientFD2, clientFD3;
	ServerResponse response;
	memset(response.EM, 0, 12 * sizeof(int));
	char hostName[100];
	memset(hostName, 0, 100);
	int port;
	int serverFD;
	int port1, port2, port3;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress1, clientAddress2, clientAddress3;

	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw runtime_error("Socket failure");
	}

	port = atoi(argv[1]);

	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port1);

	if (bind(serverFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		throw runtime_error("Unable to bind the address to the file descriptor");
	}

	if(listen(serverFD, 3) < 0)
	{
		throw runtime_error("Listening failed");
	}

	cout << "Listening for clients..." << endl;

	struct sockaddr_in clientAddress1, clientAddress2, clientAddress3;
	int clientAddressLength1 = sizeof(clientAddress1);
	int clientAddressLength2 = sizeof(clientAddress2);
	int clientAddressLength3 = sizeof(clientAddress3);
	
	if (clientFD1 = accept(serverFD, (sockaddr *)&clientAddress1, (socklen_t *)&clientAddressLength1 < 0)))
	    {
		    throw runtime_error("Error accepting client 1 to the server"); 
	    }
		
	if (clientFD2 = accept(serverFD, (sockaddr *)&clientAddress2, (socklen_t *)&clientAddressLength2 < 0)))
	    {
		    throw runtime_error("Error accepting client 2 to the server");	    
	    }
	if (clientFD3 = accept(serverFD, (sockaddr *)&clientAddress3, (socklen_t *)&clientAddressLength3 < 0)))
	    {
		    throw runtime_error("Error accepting client 3 to the server");	    
	    }

	read(clientFD1, &request[0].beginningProcess, sizeof(request[0].beginningProcess));
	read(clientFD1, &request[0].endingProcess, sizeof(request[0].endingProcess));
	read(clientFD1, &request[0].valueToBinary, sizeof(request[0].valueToBinary));
	
	read(clientFD2, &request[1].beginningProcess, sizeof(request[1].beginningProcess));
	read(clientFD2, &request[1].endingProcess, sizeof(request[1].endingProcess));
	read(clientFD2, &request[1].valueToBinary, sizeof(request[1].valueToBinary));
	
	read(clientFD1, &request[2].beginningProcess), sizeof(request[2].beginningProcess));
	read(clientFD1, &request[2].endingProcess), sizeof(request[2].endingProcess));
	read(clientFD1, &request[2].valueToBinary), sizeof(request[2].valueToBinary));
	
	encode(request.beginningProcess, request.value);
	
	write(clientFD1, &response.EM, sizeof(response.EM);
	if(request[0].beginningProcess == 1)
	{
		write(clientFD1, &w1, sizeof(w1);
	}
	else if(request[0].beginningProcess == 2)
	{
		write(clientFD1, &w2, sizeof(w2);		      
	}
	else if(request[0].beginningProcess == 3)
	{
		write(clientFD1, &w3, sizeof(w3);
	} 
		      
	sleep(1);

	write(clientFD2, &response.EM, sizeof(response.EM);
	      
	if(request[1].beginningProcess == 1)
	{
		write(clientFD1, &w1, sizeof(w1);
	}
	else if(request[1].beginningProcess == 2)
	{
		write(clientFD1, &w2, sizeof(w2);		      
	}
	else if(request[1].beginningProcess == 3)
	{
		write(clientFD1, &w3, sizeof(w3);
	} 
		      
	sleep(1);

	write(clientFD3, &response.EM, sizeof(response.EM);
	      
	if(request[2].beginningProcess == 1)
	{
		write(clientFD1, &w1, sizeof(w1);
	}
	else if(request[2].beginningProcess == 2)
	{
		write(clientFD1, &w2, sizeof(w2);		      
	}
	else if(request[2].beginningProcess == 3)
	{
		write(clientFD1, &w3, sizeof(w3);
	} 
		      
	sleep(1);

	close(clientFD1);
	close(clientFD2);
	close(clientFD3);
	close(serverFD);

	return 0;
}
