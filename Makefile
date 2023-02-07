all:
        gcc -Wall -g -o server_main server_main.c

server:
        gcc -Wall -g -o server_main server_main.c

clean:
        rm -f server_main
