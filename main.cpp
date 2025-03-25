#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <string>

using namespace std;

string requesttype;

string getCurrentDate() {
    time_t now = time(0);
    struct tm tm_struct;
    gmtime_r(&now, &tm_struct);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm_struct);
    return string(buffer);
}

int main() {
    cout << "Creating socket" << endl;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8081);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    istringstream stream(buffer);
    string line;

    while (getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        if (line.rfind("GET ", 0) == 0) {
            cout << "GET request detected!\n";
            requesttype = "GET";
        }
         if (line.rfind("Host ", 0) == 0) {
            cout << "Connection from host: " << line;
        }
    }
    if (requesttype == "GET") {
    string date = getCurrentDate();
    string message =
        "HTTP/1.1 200 OK\r\n"
        "Date: " + date + "\r\n"
        "Server: SimpleC++Server\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 85\r\n"
        "\r\n"
        "<html>\n<head><title>Welcome</title></head>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";

    send(clientSocket, message.c_str(), message.size(), 0);
    }

    close(serverSocket);

    return 0;
}
