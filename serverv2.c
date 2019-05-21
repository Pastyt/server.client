#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define maxusercount 10
#define bufsize 1024
struct sock {
        int socket;
        char nickname[10];
};
int main(){
        int welcomeSocket, newSocket;
        char buffer[bufsize],nick_buf[1024],who[10],ex[10];
        strcpy(who,"/who");
        strcpy(ex,"/exit");
        struct sockaddr_in serverAddr;
        struct sockaddr_storage serverStorage;
        socklen_t addr_size;
        welcomeSocket = socket(PF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(7891);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
        bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        addr_size = sizeof serverStorage;
        int users_count=0,test,skipper;
        struct sock users[maxusercount];
        if(listen(welcomeSocket,5)==0)
                printf("Listening\n");
        else
                printf("Error\n");
        while (1) {
                skipper=0;
                newSocket = -1;
                printf("Searching for task\n");
                while (newSocket == -1) {
                        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
                        if(users_count>0) {
                                for (int i = 0; i < users_count; i++) {
                                        if (recv(users[i].socket, nick_buf, 10, MSG_DONTWAIT) > 0) {
                                                newSocket=users[i].socket;
                                                skipper=1;
                                                break;
                                        }
                                }
                        }
                        usleep(100000);
                }
                printf("Searching complete\n");
                if (skipper == 0) {
                        test=0;
                        for (int i = 0; i < users_count; i++) {
                                if( users[i].socket == newSocket )
                                        test=1;
                        }
                        if (test==0) { /* NEW USER */
                                printf("Registration for new user\n");
                                users[users_count].socket=newSocket;
                                recv(newSocket, buffer, bufsize, 0);
                                strcpy(users[users_count].nickname,buffer);
                                strcpy(buffer,"Registration complete");
                                send(newSocket,buffer,bufsize,0);
                                users_count++;
                        }

                        strcpy(buffer,"Write a message to someone.");
                        send(newSocket,buffer,bufsize,0);
                        recv(newSocket, nick_buf, 10, 0);
                }
                if ( strcmp(nick_buf,who) == 0 ) {
                        printf("Current users requesed\n");
                        char a=users_count+'0';
                        strcpy(buffer,"Online on server . user. In list:");
                        buffer[17]=a;
                        send(newSocket,buffer,bufsize,0);
                        for (int i = 0; i < users_count; i++)
                                send(newSocket,users[i].nickname,bufsize,0);
                } else if ( strcmp(nick_buf,ex) == 0 ) { /* USERS OR EXIT */
                        printf("Exit requesed\n");
                        close (newSocket);
                        for (int i = 0; i < users_count-1; i++) {
                                if( users[i].socket == newSocket) {
                                        while( i < users_count ) {
                                                users[i].socket=users[i+1].socket;
                                                strcpy(users[i].nickname,users[i+1].nickname);
                                                i++;
                                        }
                                        break;
                                }
                        }
                        users_count--;
                } else {
                        recv(newSocket, buffer, bufsize, 0);
                        printf("Messages received\n");
                        test=0;
                        for (int i = 0; i < users_count; i++) {
                                if( strcmp(users[i].nickname,nick_buf)== 0  ) {
                                        for (int j = 0; j < users_count; j++)
                                                if (users[j].socket==newSocket)
                                                        send(users[i].socket,users[j].nickname,bufsize,0);

                                        send(users[i].socket,buffer,bufsize,0);
                                        test=1;
                                }
                        }
                        if (test == 0) {
                                strcpy(buffer,"Nick ain't found. Message delete.");
                                send(newSocket,buffer,bufsize,0);
                        }
                }
        }
        return 0;
}
