#include <iostream>
#include <string>
using namespace std;

// you can define SOCKET_READ_BUFFER_SIZE to override default value 1024
#include "Socket.cpp"

int main(int argc, char *argv[])
{
    cout << "Opening port... ";
    FILE *host = socket_listen(5005);
    cout << "DONE." << endl;

    bool end = false;

    while (!end) {

        // wait for connection
        cout << "Waiting for connection... "; cout.flush();
        FILE *connection = socket_accept(host);
        cout << "connection established." << endl;

        //////////////////////
        // Communicate

        SocketStream sock(connection);
        
        cout << "read" << endl;
        string buffer;
        buffer = sock.readln();
        cout << "Received: " << buffer << endl;

        if (buffer[0] == 'q')
            end = true;
        
        // sleep
        usleep(1000000);

        // write
        cout << "write:" << endl;
        sock.writeln("I got your message");

        // write again
        cout << "write:" << endl;
        sock.writeln("Your message was: "+buffer);
        
        fclose(connection);
    }

    fclose(host);
    
    return 0; 
}