#include <sys/socket.h>  
#include <sys/types.h>  
#include <resolv.h>  
#include <string.h>  
#include <pthread.h>  
#include <unistd.h>  
#include <stdlib.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *runSocket(void *item)  
{  
    int c_fd =(int)item; 
    char buffer[BUF_SIZE];  
    int bytes = 0;

    while(1)  
    {  
        memset(&buffer,'\0',sizeof(buffer));  
        bytes = read(c_fd, buffer, sizeof(buffer));  
        if(bytes <= 0)  
        {  
            perror("Failed to read\n");
	    exit(1);
        }  
        
	else if(bytes > 0){
	    write(c_fd, buffer, sizeof(buffer));  
        }
    }       
    return NULL;  
}
  
int main()  
{  
    int client_fd;  
    char buffer[SMALL_BUF];  
    int fd = 0 ;  
    struct sockaddr_in server_sd;

    // Create a socket  
    fd = socket(AF_INET, SOCK_STREAM, 0);  
    printf("Server started\n");  
    memset(&server_sd, 0, sizeof(server_sd));  
    
    server_sd.sin_family = AF_INET;  
    server_sd.sin_port = htons(5010);  
    server_sd.sin_addr.s_addr = INADDR_ANY;  
    
    // Bind the socket 
    bind(fd, (struct sockaddr*)&server_sd,sizeof(server_sd));  
    
    // Listening for new connections
    listen(fd, SOMAXCONN);  
    
    while(1)  
    {  
        client_fd = accept(fd, (struct sockaddr*)NULL ,NULL);  
        if(client_fd < 0)
	{
	    perror("Failed to accept\n");
	    exit(1);
	}
	else if(client_fd > 0){
	    printf("accepted client no. : %d\n",client_fd);  
        
            printf("Proxy connected\n");     
            pthread_t tid;  
            pthread_create(&tid, NULL, runSocket, (void *)client_fd);   
        }
    } 
    close(client_fd);
    close(fd);
    return 0;  
}  
