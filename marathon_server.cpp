#include <iostream>
#include <string>
using namespace std;

// you can define SOCKET_READ_BUFFER_SIZE to override default value 1024
#include "Socket.cpp"

int main(int argc, char *argv[])
{
    FILE *host = socket_listen(5005);

    while (true) { // fuck it

        // wait for connection
        cout << "Waiting for connection... "; cout.flush();
        FILE *connection = socket_accept(host);
        cout << "connection established." << endl;

        //////////////////////
        // Communicate

        SocketStream sock(connection);
        
        // Auth
        sock.writeln("LOGIN");
        string login = sock.readln();
        login.pop_back(); // trim \n

        sock.writeln("PASSWORD");
        string password = sock.readln();
        password.pop_back(); // trim \n

        if (login != "bacon" || password != "*****") {
            sock.writeln("ERROR 1 Incorrect login or password.");
            fclose(connection);
            continue;
        }
        else
            sock.writeln("OK");


        // message loop
        while (true) {

            string msg = sock.readln();
            msg.pop_back(); // trim \n
            cout << "received: " << msg << endl;

            if (msg == "QUIT") { // extra command
                sock.writeln("OK");
                break;
            }
            else if (msg == "GET_TIME") {
                sock.writeln("OK");
                sock.writeln("1 1 120");
            }
            else if (msg == "WAIT") {
                sock.writeln("OK");
                sock.writeln("WAITING");
                usleep(2000000); // such waiting
                sock.writeln("OK");
            }
            else if (msg == "GET_SCORE") {
                sock.writeln("OK");
                sock.writeln("65");
            }
            else if (msg == "GET_ALL_SCORES") {
                sock.writeln("OK");
                sock.writeln("6");
                sock.writeln("123 65 32 3 3 0");
            }
            else {
                sock.writeln("ERROR 3 Unknown command name.");
            }

        }
        
        fclose(connection);
    }

    fclose(host);
    
    return 0; 
}