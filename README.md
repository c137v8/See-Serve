# Learning Buffer Overflow with a Simple C++ HTTP Server

## Introduction

This project is a simple C++ HTTP server designed for learning about buffer overflows and how they can be exploited. The goal is to understand how unprotected memory operations can lead to security vulnerabilities and how to prevent them.

## Features

- Handles HTTP GET and POST requests
- Serves a basic login page
- Hardcoded username and password for authtication
- Improper memery management 
- Simple authenticated page

## How It Works

- The server listens on port 8081 for incoming connections.
- On a 'GET' request , it serves a simple html login page.
- On a `POST` request, server extracts username and password, which are then authnticated.

## Understanding Buffer Overflow

The server uses a fixed-size buffer (`char buffer[1024]`), but `recv()` is called with a larger size (`2048`), leading to a  buffer overflow vulnerability.

## Future Implimentations

- Code to craft a POST request that bypasses authentication.

## Running the Server

1. Compile the server using g++:
   ```sh
   g++ -o server main.cpp
   ```
2. Run the server:
   ```sh
   ./server
   ```
3. Open a web browser and go to `http://127.0.0.1:8081`

##

