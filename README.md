# TCP Port Scanner
A multithreaded TCP port scanner built in C++ using Winsock2.

## How to compile
```bash
g++ main.cpp os_detection.cpp -o scanner -lws2_32 -liphlpapi
```

## How to use
```bash
./scanner
```
Then enter the target IP and port range when prompted.

## Features
- Multithreaded scanning (4 threads)
- OS detection via TTL fingerprinting
- Banner grabbing
