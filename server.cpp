#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using namespace std;


int main()
{
  char dataSending[1025]; // Actually this is called packet in Network Communication, which contain data and send through.
	int clientListn = 0; 
  int clientConnt = 0;
	struct sockaddr_in ipOfServer;
	clientListn = socket(AF_INET, SOCK_STREAM, 0); // creating socket
	memset(&ipOfServer, '0', sizeof(ipOfServer));
	memset(dataSending, '0', sizeof(dataSending));
	ipOfServer.sin_family = AF_INET;
	ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
	ipOfServer.sin_port = htons(2017); 		// this is the port number of running server
	bind(clientListn, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));
	listen(clientListn , 20);
 
	while(1)
	{
		cout <<"Hi,Iam running server.Some Client hit me\n"; // whenever a request from client came. It will be processed here.
		clientConnt = accept(clientListn, (struct sockaddr*)NULL, NULL);
 
		write(clintConnt, dataSending, strlen(dataSending));
 
        close(clientConnt);
        sleep(1);
     }
 
     return 0;
}
