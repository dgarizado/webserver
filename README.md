# 🖥️ 42 Webserv

A lightweight HTTP server implemented in C++98, capable of handling multiple clients using non-blocking sockets and the `select()` system call.

## 🎯 Project Overview

The 42 Webserv project aims to build a fully functional HTTP/1.1 server from scratch. It focuses on:

- Low-level socket programming
- Non-blocking I/O
- HTTP protocol handling
- Multi-client concurrent connections
- Serving static and dynamic content (CGI)

## 🔧 Core Features

- Supports HTTP methods: GET, POST, PUT, DELETE, HEAD
- Non-blocking I/O with `select()`
- Handles multiple clients simultaneously
- Serves static files from a configured root directory
- CGI integration for dynamic content
- Custom configuration file parsing
- Robust error handling and HTTP status codes


