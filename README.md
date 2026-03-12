# TCP Port Scanner

A multithreaded TCP port scanner built in C++ using Winsock2.

## How to compile
```bash
g++ main.cpp -o scanner -lws2_32
```

## How to use
```bash
./scanner
```

Then enter the target IP and port range when prompted.

## Features

- Multithreaded scanning (4 threads)

## Planned features

- Save results to file
- OS detection
