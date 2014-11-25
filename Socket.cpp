#include <stdio.h> 
#include <unistd.h> // read(), write() unix functions
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <string>
using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(0);
}

/**
 * Client: Connect to open socket.
 * Returns: FILE*
 */
FILE *socket_connect(const char *host, int port) {
    int sockfd;   // socket file descriptor
    struct hostent *server;
    struct sockaddr_in serv_addr;

    // create a tcp socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    // get server
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host\n");
 
    // serv_addr setup
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(port);

    // connect to the server
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // attach to a file stream
    return fdopen(sockfd, "r+");
}

/**
 * Server: listen on a port
 */
 FILE *socket_listen(int port) {
     // create socket file descriptor
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    
    // open port setup
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // bind and listen
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);

    // attach to a file stream
    return fdopen(sockfd, "r+");
}

/**
 * Server: accept connection on a socket
 */
 FILE *socket_accept(FILE *socket) {
    // create socket file descriptor
    int newsockfd;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    newsockfd = accept(fileno(socket), (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");

    // attach to a file stream
    return fdopen(newsockfd, "r+");
}

#ifndef SOCKET_READ_BUFFER_SIZE
#define SOCKET_READ_BUFFER_SIZE 1024
#endif

class SocketStream {
private:
    FILE *socket;
    char read_buffer[SOCKET_READ_BUFFER_SIZE];

public:

    SocketStream(FILE *stream) {
        socket = stream;
    }

    void writeln(string str) {
        if (fprintf(socket, "%s\n", str.c_str()) < 0)
            error("ERROR when writing");
        // VZDY po sebe splachni!
        fflush(socket);
    }

    string readln() {
        if (fgets(read_buffer, sizeof(read_buffer)-1, socket) == NULL)
            error("ERROR when reading");
        return string(read_buffer);
    }
};
