#include <iostream>
#include <string>
using namespace std;

// you can define SOCKET_READ_BUFFER_SIZE to override default value 1024
#include "Socket.cpp"

int main(int argc, char *argv[])
{
    FILE *sockFile = socket_connect("127.0.0.1", 5005);
    SocketStream sock(sockFile);

    //////////////////////////////////
    // Communication

    string buffer;

    // write
    cout << "Please enter the message: ";
    cin >> buffer;
    sock.writeln(buffer);

    // sleep
    //usleep(1000);

    // read
    cout << sock.readln();

    // read
    cout << sock.readln();
    
    return 0;
}