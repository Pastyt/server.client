#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define nicksize 10
#define bufsize 1024
int main(){
        int clientSocket;
        char buffer[bufsize],nick[10],who[4],ex[5];
        int key=3;
        strcpy(who,"/who");
        strcpy(ex,"/exit");
        struct sockaddr_in serverAddr;
        socklen_t addr_size;
        int getlinesize=1024;
        clientSocket = socket(PF_INET, SOCK_STREAM, 0);

        serverAddr.sin_family = AF_INET;

        serverAddr.sin_port = htons(7891);

        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
        int i=0;
        addr_size = sizeof serverAddr;
        connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
        if (fork()==0) {
                printf("Connect established\n");
                printf("Write your nickname for server:\n");
                scanf("%s",buffer);
                send(clientSocket,buffer,bufsize,0);
                printf("Write a message or who to see users or command\n");
                printf("Nick/space/Message\n");
                while(1) {
                        scanf("%s",nick);
                        if (strcmp(nick,who)==0) {
                                send(clientSocket,nick,10,0);
                                /* IF NEED USERS */
                        } else if (strcmp(nick,ex) == 0) {
                                /* IF NEED EXIT */
                                send(clientSocket,nick,10,0);
                                close(clientSocket);
                                return 0;
                        } else {
                                /* IF MESSAGE */
                                send(clientSocket,nick,10,0);
                                scanf("%s",buffer);
                                /*for (int i = 0; i < 30; i++) {
                                        buffer[i]+=key*2;
                                } */
                                send(clientSocket,buffer,bufsize,0);
                                /* Recv in fork */
                        }
                }

        } else {
                while(1) {
                        recv(clientSocket, buffer, bufsize, 0);
                        printf("%s\n",buffer);
                        /*    for (int i = 0; i < 30; i++)
                                    buffer[i]-=key*2;

                            printf("%s\n",buffer);
                         */
                }

                return 0;
        }
}
