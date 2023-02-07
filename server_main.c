#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

int socketSetup(int sock, int port_number) {
    struct sockaddr_in myaddr;

    myaddr.sin_port= htons(port_number);
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    return bind(sock, (struct sockaddr*)&myaddr, sizeof(myaddr));
}

int parse_argument(int argc, char **argv, int *port_number, char **document_root) {
    // Checks to see that the number of arguments is correct
    if(argc != 5) {
      printf("The number of arguments entered is incorrect.\n");
      return -1;
    }
  
    // This tells us what flag we are reading and how to interpret the next string
    int flag_type = -1;
    int args_added = 0;
    
    // Start from 1 because first arg is always the program name
    for(int i = 1; i < argc; i++) {
      if(argv[i][0] == '-') {
            if(argv[i][1] == 'p') {
                flag_type = 1;
            } else if(argv[i][1] == 'd') {
                flag_type = 2;
            } else {
                flag_type = -1;
            }
        } else if(flag_type == 1) {
            *port_number = atoi(argv[i]);
            args_added++;
	        if(*port_number > 9999 || *port_number < 8000) {
	            printf("Invalid port number\n");
	            return -1;
	        }
        } else if(flag_type == 2) {
	        *document_root = argv[i];
            args_added++;
        } else {
            printf("Either an unsupported flag type was entered or there was no flag before a parameter\n");
            return -1;
        }
    }

    if(args_added != 2) {
        printf("Not all arguments were read correctly.\n");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int port_number;
    char *document_root;

    if(parse_argument(argc, argv, &port_number, &document_root) < 0) {
        printf("There was an error parsing the inputs. Please use -document_root and -port flag followed by the arguments.\n");
        return -1;
    }

    printf("Success, the port number is %i and the file path is %s\n", port_number, document_root);

    int sock = socket(AF_UNIX, SOCK_STREAM, IPPROTO_TCP);
    int successful_bind = socketSetup(sock, port_number);

    printf("was successful? %i with socket number %i\n", successful_bind, sock);

    close(sock);

    return 0;
}
