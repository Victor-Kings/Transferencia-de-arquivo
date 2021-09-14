#include <ifaddrs.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    char version[4];
    char headerLength[4];
    char serviceType[8];
    char totalLength[16];
    char identification[16];
    char flags[4];
    char fragmentOffset[12];
    char timeToLife[8];
    char protocol[8];
    char headerCheckSum[16];
    char sourceIpAddress[32];
    char destinationIp[32];
    char options[32];
    char data[32];
}PackageIP;

PackageIP * createIPPackage() {
    PackageIP * package = malloc(sizeof(PackageIP));
    return package;
}

void setIP(char* destinationIP, PackageIP* package ) {


    memcpy( package->destinationIp, destinationIP, 32);
    int n;
    struct ifreq ifr;
    char array[] = "eth0";
 
    n = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);
    
    memcpy( package->sourceIpAddress, inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr),32);
}