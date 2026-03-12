#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>

std::mutex mtx;
std::vector<std::pair<int, bool>> results; // port, open/closed

void scan(int start_port, int end_port, sockaddr_in service){
    for(int port = start_port; port <= end_port; port++){
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock == INVALID_SOCKET){
            WSACleanup();
            return;
        }
        service.sin_port = htons(port);
        bool open = connect(sock, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) != SOCKET_ERROR;

        std::lock_guard<std::mutex> lock(mtx);
        results.push_back({port, open});

        closesocket(sock);
    }
}

int main(){
    WSADATA wsa_data;
    WORD wsa_version = MAKEWORD(2, 2);
    if(WSAStartup(wsa_version, &wsa_data) != 0){
        std::cout << "Winsock init failed!\n";
        return 0;
    }

    std::string ip;
    int start_port, end_port;
    std::cout << "IP Address: ";
    std::cin >> ip;
    std::cout << "Enter the start port: ";
    std::cin >> start_port;
    std::cout << "Enter the end port: ";
    std::cin >> end_port;

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip.c_str());

    int chunk = (end_port - start_port) / 4;

    std:: cout << "Scanning..." << '\n';

    std::thread t1(scan, start_port, start_port + chunk, service);
    std::thread t2(scan, start_port + chunk + 1, start_port + chunk * 2, service);
    std::thread t3(scan, start_port + chunk * 2 + 1, start_port + chunk * 3, service);
    std::thread t4(scan, start_port + chunk * 3 + 1, end_port, service);

    t1.join(); t2.join(); t3.join(); t4.join();

    std::sort(results.begin(), results.end());

    for(auto& [port, open] : results){
        std::cout << "Port " << port << (open ? " is open!" : " is closed!") << '\n';
    }


    std::cout << "Port scanning is done!\n";
    WSACleanup();
    return 0;
}
