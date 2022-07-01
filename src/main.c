#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


char client_message[256] = "Hello";

int sockfd;
struct sockaddr_in client_addr, server_addr;
socklen_t cLen = sizeof(client_addr);



void create_udp_server(int port, const char* server_ip){
    memset(&client_message, '\0', sizeof(client_message));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port); //htons means convert a number into 16-bit network representation
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    //Create socket here
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){  // int sockfd = socket(domain, type, protocol) and AF_INET uses IPv4 and SOCK_DGRAM uses Datagram Protocol=UDP
        printf("Cannot create socket \n");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully and ready to recieve message! \n");
    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
        perror("Binding the socket failed, cannot recieve message!");
        exit(EXIT_FAILURE);
    }
    printf("Successfully binded the socket \n");

    //RECIEVE MESSAGE HERE
    if(recvfrom(sockfd, client_message, sizeof(client_message), MSG_WAITALL, (struct sockaddr*) &client_addr, &cLen)<0){
        printf("Cannot recieve the message! \n");
    }
    printf("Message from client: %s \n",client_message);
    close(sockfd);
    exit(0);
}

char server_ip[256];
int port;

void load_server_configuration(){
    char line[256];
    FILE* config_file = fopen("starter.config", "r");

    char* parameter;
    char* value;
    char* token;
    const char* delimiter_characters = " ";

    if(config_file == NULL)
    {
        fprintf( stderr, "Unable to open the config file");
    }
    else
    {
        fgets(line, 256, config_file);
        int count = 0;
        token = strtok(line, delimiter_characters);
        while(token != NULL && count != 2)
        {
            if(count != 0){
                token = strtok(NULL,delimiter_characters);
            }
            strcpy(parameter, token);
            token = strtok(NULL,delimiter_characters);
            strcpy(value, token);
            
            if(strcmp(parameter,"SERVER_IP")==0){
                strcpy(server_ip, value);
            }
            else if(strcmp(parameter,"PORT")==0){
                sscanf(value, "%d", &port);
            }
            count++;
        }
    }    
    printf("Server ip: %s\n",server_ip);
    printf("Port: %d\n",port);
    printf( "----------------------\n");      
    
    fclose(config_file);

}
int main(){

    load_server_configuration();
    create_udp_server(port, server_ip);
    return 0;

}