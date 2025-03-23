    #include <cstring>
    #include <iostream>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <sstream>
    using namespace std;

    int main()
    {
        cout << " Creating socket" << endl;
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8081);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        bind(serverSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

        listen(serverSocket, 5);

        // accepting connection request
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        // recieving data
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
         istringstream stream(buffer);
        string line;

    // Read line by line
         while (getline(stream, line)) {
          cout << "Read line: " << line << endl;
         }
            const char* message =
        "HTTP/1.1 200 OK\r\n"
        "Date: Sun, 23 Mar 2025 12:34:56 GMT\r\n"
        "Server: SimpleC++Server\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 85\r\n"
        "\r\n"
        "<html>\n<head><title>Welcome</title></head>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";

    send(clientSocket, message, strlen(message), 0);
        // closing the socket.
        close(serverSocket);

        return 0;
    }
