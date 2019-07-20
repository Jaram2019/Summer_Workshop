#include <sys/socket.h>  
#include <sys/types.h>  
#include <resolv.h>  
#include <string.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
 
#define BUF_SIZE 1024
#define SMALL_BUF 100

struct serverInfo  
{  
    int client_fd;  
    char ip[SMALL_BUF];  
    char port[SMALL_BUF];  
};  

void *runSocket(void *item)  
{  
    struct serverInfo *info = (struct serverInfo *)item;  
    char buffer[BUF_SIZE];  
    int bytes = 0;  

    printf("Client no. : %d\n",info->client_fd);  
    printf("IP address  : %s\n", info->ip);  
    printf("Port number : %s\n", info->port);  
    
    int server_fd = 0;  
    struct sockaddr_in server_sd;  
    
    // Create a socket  
    server_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if(server_fd < 0)  
    {  
        perror("Failed to create a Server socket\n");
	exit(1);
    }

    printf("A server socket created\n");       
    memset(&server_sd, 0, sizeof(server_sd));  
    
    server_sd.sin_family = AF_INET;  
    server_sd.sin_port = htons(atoi(info->port));  
    server_sd.sin_addr.s_addr = inet_addr(info->ip);  
    
    // Connect to Main Server  
    if((connect(server_fd, (struct sockaddr *)&server_sd, sizeof(server_sd)))<0)  
    {  
        perror("Failed to connect to main server\n");  
        exit(1);
    }  
    printf("Connect Success\n");  
    
    while(1)  
    {  
        memset(&buffer, '\0', sizeof(buffer));  
        bytes = read(info->client_fd, buffer, sizeof(buffer));   
	if(bytes > 0)   
        { 
            printf("From Client : %s\n", buffer);
            strcat(buffer, " from Client to Server\n");
	    write(server_fd, buffer, sizeof(buffer));  
        }

        memset(&buffer, '\0', sizeof(buffer));  
        bytes = read(server_fd, buffer, sizeof(buffer));  
        if(bytes > 0) 
        {  
            printf("From main Server : %s\n", buffer);
	    strcat(buffer, " from Server to Client\n");
            write(info->client_fd, buffer, sizeof(buffer));  
        }
    }
    close(server_fd);
    return NULL;  
}  

int main(int argc,char *argv[])  
{  
    pthread_t tid;  
    char port[SMALL_BUF], ip[SMALL_BUF];  
    char proxy_port[SMALL_BUF];  

    strcpy(ip,argv[1]); // server ip  
    strcpy(port,argv[2]);  // server port  
    strcpy(proxy_port,argv[3]); // proxy port
    printf("Server ip address : %s\n", ip);
    printf("Server port number : %s\n", port);
    printf("Proxy Server port number : %s\n", proxy_port);
    
    int proxy_fd =0, client_fd=0;  
    struct sockaddr_in proxy_sd;  
    
    // Create a socket  
    if((proxy_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
    {  
        perror("Failed to create socket\n");
	exit(1);
    }  

    printf("Proxy created\n");  
    memset(&proxy_sd, 0, sizeof(proxy_sd));  
    
    proxy_sd.sin_family = AF_INET;  
    proxy_sd.sin_port = htons(atoi(proxy_port));  
    proxy_sd.sin_addr.s_addr = INADDR_ANY;  
    
    // Bind the socket  
    if((bind(proxy_fd, (struct sockaddr*)&proxy_sd,sizeof(proxy_sd))) < 0)  
    {  
        perror("Failed to bind a socket\n");  
        exit(1);
    }  

    // listen for new connections
    if((listen(proxy_fd, SOMAXCONN)) < 0)  
    {  
        perror("Failed to listen\n");  
        exit(1);
    }  
    printf("Listening\n");  
    
    while(1)  
    {  
        client_fd = accept(proxy_fd, (struct sockaddr*)NULL ,NULL);  
        if(client_fd < 0)
	{
	    perror("Failed to accept\n");
	    exit(1);
	}

	printf("client no. %d connected\n",client_fd);  
        
        struct serverInfo *item = malloc(sizeof(struct serverInfo));  
        item->client_fd = client_fd;  
        strcpy(item->ip,ip);  
        strcpy(item->port,port);  
        pthread_create(&tid, NULL, runSocket, (void *)item);  
        
    }  
    close(client_fd);
    close(proxy_fd);
    return 0;  
}
