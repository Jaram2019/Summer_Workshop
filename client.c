#include <sys/socket.h>  
#include <sys/types.h>  
#include <resolv.h>  
#include <string.h>  
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

int main(int argc, char* argv[])  
{  
    char ip[SMALL_BUF];  
    char port[SMALL_BUF];  
    char buffer[BUF_SIZE];  
    int sd;  
    struct sockaddr_in client_sd;  
    
    printf("\nEnter proxy ip address : ");
    fgets(ip,sizeof("127.0.01\n")+1,stdin); 
    printf("\nEnter a port number : ");
    fgets(port,sizeof("5000\n")+1,stdin); 

    if((strcmp(ip,"127.0.0.1\n"))!=0 || (strcmp(port,"5000\n"))!=0)  
    {  
        perror("Invalid ip or port number\n");  
        exit(1);
    }  
    
    // Create a socket  
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
    {  
        perror("Failed to create socket\n");  
        exit(1);
    } 

    memset(&client_sd, 0, sizeof(client_sd));  
    
    // Set sockaddr_in  
    client_sd.sin_family = AF_INET;  
    client_sd.sin_port = htons(5000);  
    client_sd.sin_addr.s_addr = INADDR_ANY;   
    
    // Connect to proxy server
    connect(sd, (struct sockaddr *)&client_sd, sizeof(client_sd));  
    
    while(1)  
    {  
	printf("\nEnter your message : ");
        fgets(buffer, sizeof(buffer), stdin);  
	write(sd, buffer, sizeof(buffer));  
        
	read(sd, buffer, sizeof(buffer));  
        printf("Response : %s\n", buffer);
    }  
    close(sd);
    return 0;  
}  
