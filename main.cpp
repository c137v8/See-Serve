#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <string>

using namespace std;


string getCurrentDate() {
    time_t now = time(0);
    struct tm tm_struct;
    gmtime_r(&now, &tm_struct);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm_struct);
    return string(buffer);
}
string postData;
bool isBody = false;
bool isAuthenticated = false;
string responseContent;
string requesttype;

int main() {
    cout << "Creating socket" << endl;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8081);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);
  while (true) {  
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            cerr << "Error accepting connection!" << endl;
            continue;
        }

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    //recv(clientSocket, buffer, 2048, 0);
    cout << "Recived data" << endl;
    istringstream stream(buffer);
    string line;
     string date = getCurrentDate();
    while (getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        cout << line ;
        
        if (line.rfind("GET ", 0) == 0) {
            cout << "Method: GET\n";
            requesttype = "GET";
        }
          if (line.rfind("POST ", 0) == 0) {
            cout << "Method: POST\n";
            requesttype = "POST";
        }
         if (line.rfind("Host ", 0) == 0) {
            cout << "Connection from host: " << line;
        }
            if (line.empty()) {
            isBody = true;
            continue;
        }
        if (isBody) {
            postData = line;
        }
    }
    if (requesttype == "GET") {
     date = getCurrentDate();
   string htmlContent = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login</title>
</head>
<body>
    <h2>Login</h2>
    <form action="http://127.0.0.1:8081" method="POST">
        <label for="username">Username:</label>
        <input type="text" id="username" name="username" required>
        <br><br>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required>
        <br><br>
        <button type="submit">Login</button>
    </form>
</body>
</html>)";

            string response =
                "HTTP/1.1 200 OK\r\n"
                "Date: " + date + "\r\n"
                "Server: SimpleC++Server\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: " + to_string(htmlContent.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" +
                htmlContent;


    send(clientSocket, response.c_str(), response.size(), 0);
    }
    if(requesttype== "POST"){
         cout << "Received POST Data: " << postData << endl;
        
        // Extract username and password
        string username, password;
        size_t userPos = postData.find("username=");
        size_t passPos = postData.find("&password=");

        if (userPos != string::npos && passPos != string::npos) {
            username = postData.substr(userPos + 9, passPos - (userPos + 9));
            password = postData.substr(passPos + 10);
        }

        cout << "Extracted Username: " << username << endl;
        cout << "Extracted Password: " << password << endl;
        if ( username == "admin" && password == "virat2025"){
        isAuthenticated = true; 
        }
        if( isAuthenticated){
         date = getCurrentDate();
        string responseContent = "<html><body><h2>Login Successful</h2><p>Welcome, " + username + "!</p></body></html>";
        }
        else if (!isAuthenticated) {
              date = getCurrentDate();
        string responseContent = "<html><body><h2>Wrong password</h2><p>not Welcome, " + username + "!</p></body></html>";

        }
        string response =
            "HTTP/1.1 200 OK\r\n"
            "Date: " + date + "\r\n"
            "Server: SimpleC++Server\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: " + to_string(responseContent.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            responseContent;

        send(clientSocket, response.c_str(), response.size(), 0);
  

    }
    close(clientSocket);
  }
    close(serverSocket);

    return 0;
}
