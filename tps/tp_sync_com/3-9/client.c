#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <string.h>

int main(int argc, char const* argv[]) 
{
	puts("This client version only accept the USERNAME as param :(\n");
	puts("I invite you to collaborate in order to allow the client ask for the SERVER_IP and SERVER_PORT\n");
	puts("So we can call the program as follow: ./client.out $SERVER_IP, $SERVER_PORT, $USERNAME\n");
	if (argc < 2)
    {
        printf("Usage: %s <your_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *username = argv[1];

	int sockD = socket(AF_INET, SOCK_STREAM, 0); 

	struct sockaddr_in servAddr; 

	servAddr.sin_family = AF_INET; 
	servAddr.sin_port 
		= htons(9001); // use some unused port number 
	servAddr.sin_addr.s_addr = INADDR_ANY; 

	int connectStatus 
		= connect(sockD, (struct sockaddr*)&servAddr, 
				sizeof(servAddr)); 

	if (connectStatus == -1) { 
		printf("Error...\n"); 
	} 

	else {
        //const char *username = "USER";
        send(sockD, username, strlen(username), 0);

		char strData[255];
		recv(sockD, strData, sizeof(strData), 0); 

		printf("Greetings from server: %s\n", strData); 
	} 

	return 0; 
}
