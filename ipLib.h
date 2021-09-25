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
    char version[4];            //X
    char headerLength[4];       //X
    char serviceType[8];        //X
    char totalLength[16];       //X
    char identification[16];    //X
    char flags[3];              //X
    char fragmentOffset[12];    //X
    char timeToLive[8];         //X
    char protocol[8];           //X
    char headerCheckSum[16]; //x
    char sourceIpAddress[32]; //x
    char destinationIp[32]; //x
    char options[32]; //x
    char data[32]; // ???????
}PackageIP;

PackageIP * createIPPackage() {
    PackageIP * package = malloc(sizeof(PackageIP));
    return package;
}

void setVersion(PackageIP* package) {
    memcpy( package->destinationIp, "4", 4);
}

void setServiceType(PackageIP* package, char* param ) {
    memcpy( package->serviceType, param, 8);
}

void setTotalLength(PackageIP* package){
    char str[16];
    sprintf(str, "%d", sizeof(package));
    memcpy( package->totalLength, str, 16);
}

void setProtocol(PackageIP* package, char protocol) {
    char str[16];
    if(protocol=='U'){
          sprintf(str, "%d", 17);
    } else {
         sprintf(str, "%d", 6);
    }
      memcpy( package->protocol, str, 3);
}

void setOptions(PackageIP* package) {
 memcpy( package->options, "", 32);
}

void setFlags(int fragmented, int lastFragment){
    char str[4];
    str[0]='0';
    if(fragmented==1){
        str[1]='0';
    } else {
        str[1]='1';
    }

    if(lastFragment==1){
        str[2]='0'
    } else {
        str[2]='1';
    }
     memcpy( package->flags, str, 3);
}

void setFragmentOffset(PackageIP* package, char* position ) {
    memcpy( package->fragmentOffset, position, 12);
}

void setTimeToLive(PackageIP* package){
     char str[8];
    sprintf(str, "%d", 255);
    memcpy( package->timeToLive, str, 8);
}

void setIdentification(PackageIP* package, int index){
     char str[16];
    sprintf(str, "%d",index);
    memcpy( package->identification, str, 16);
}

void setHeaderLenght(PackageIP* package, int length){
     memcpy( package->headerLength, "60", 4);
}

unsigned int checksumVerify(char *buffer, size_t len, unsigned int seed){
    unsigned char *buf = (unsigned char *)buffer;
    size_t i;
    for (i = 0; i < len; ++i)
    seed += (unsigned int)(*buf++);
    return seed;
}

void setHeaderCheckSum(PackageIP* package, unsigned int seed){
    int seedCheck = checksumVerify(package, sizeof(package), seed);
    char str[16];
    sprintf(str, "%d",seedCheck);
    memcpy( package->headerCheckSum, str, 16);
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