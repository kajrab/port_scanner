#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include "../libs/os_detection.h"

std::string detect_os(const std::string& ip){
    DWORD ipAddr = inet_addr(ip.c_str());
    HANDLE hIcmp = IcmpCreateFile();

    char sendData[] = "ping";
    DWORD replySize = sizeof(ICMP_ECHO_REPLY) + sizeof(sendData);
    LPVOID replyBuffer = malloc(replySize);


    DWORD result = IcmpSendEcho(hIcmp, ipAddr, sendData, sizeof(sendData), NULL, replyBuffer, replySize, 1000);

    if(result == 0){
        free(replyBuffer);
        IcmpCloseHandle(hIcmp);
        return "Unknown";
    }

    ICMP_ECHO_REPLY* reply = (ICMP_ECHO_REPLY*)replyBuffer;

    BYTE ttl = reply->Options.Ttl;

    free(replyBuffer);
    IcmpCloseHandle(hIcmp);

    if(ttl >= 120) return "Windows";
    else if(ttl >= 60) return "Linux/macOS";
    else return "Network Device";
}
