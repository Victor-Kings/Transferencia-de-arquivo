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
#include "udpLib.h"

typedef struct {
    char version[4];            
    char headerLength[4];       
    char serviceType[8];        
    char totalLength[16];       
    char identification[16];    
    char flags[3];              
    char fragmentOffset[12];    
    char timeToLive[8];         
    char protocol[8];           
    int16_t headerCheckSum; 
    char sourceIpAddress[32]; 
    char destinationIp[32]; 
    char options[32]; 
    Package package;
}PackageIP;

PackageIP * createIPPackage() {
    PackageIP * package = malloc(sizeof(PackageIP));
    return package;
}

void setVersion(PackageIP* package) {
    memcpy( package->version, "4", 4);
}

void setServiceType(PackageIP* package, char param ) {
    char str[8];
    sprintf(str, "%c",param);
    memcpy(package->serviceType, str, sizeof(str));
}

void setTotalLength(PackageIP* package){
    char str[16];
    sprintf(str, "%d", sizeof(PackageIP));
    memcpy( package->totalLength, str, sizeof(str));
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

void setFlags(PackageIP* package,int fragmented, int lastFragment){
    char str[3];
    str[0]='0';
    if(fragmented==1){
        str[1]='0';
    } else {
        str[1]='1';
    }

    if(lastFragment==1){
        str[2]='0';
    } else {
        str[2]='1';
    }
     memcpy( package->flags, str, 3);
}

void setFragmentOffset(PackageIP* package, int position ) {
    char str[12];
    sprintf(str, "%d", position);
    memcpy( package->fragmentOffset, str, 12);
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

void setHeaderLength(PackageIP* package){
     memcpy( package->headerLength, "60", 4);
}

unsigned int checksumVerifyIP(char *buffer, size_t len, unsigned int seed){
    unsigned char *buf = (unsigned char *)buffer;
    size_t i;
    for (i = 0; i < len; ++i)
    seed += (unsigned int)(*buf++);
    return seed;
}

void setHeaderCheckSum(PackageIP* package, unsigned int seed){
    unsigned int seedCheck=getCheckSumHeader(package);

    
    package->headerCheckSum=seedCheck;


}

int getCheckSumHeader(PackageIP* package){
    int seedCheck=0;
    seedCheck += checksumVerifyIP(package->version, sizeof(package->version), seed);
    seedCheck += checksumVerifyIP(package->headerLength, sizeof(package->headerLength), seed);
    seedCheck += checksumVerifyIP(package->serviceType, sizeof(package->serviceType), seed);
    seedCheck += checksumVerifyIP(package->totalLength, sizeof(package->totalLength), seed);
    seedCheck += checksumVerifyIP(package->identification, sizeof(package->identification), seed);
    seedCheck += checksumVerifyIP(package->flags, sizeof(package->flags), seed);
    seedCheck += checksumVerifyIP(package->fragmentOffset, sizeof(package->fragmentOffset), seed);
    seedCheck += checksumVerifyIP(package->timeToLive, sizeof(package->timeToLive), seed);
    seedCheck += checksumVerifyIP(package->protocol, sizeof(package->protocol), seed);
    seedCheck += checksumVerifyIP(package->sourceIpAddress, sizeof(package->sourceIpAddress), seed);
    seedCheck += checksumVerifyIP(package->destinationIp, sizeof(package->destinationIp), seed);
    seedCheck += checksumVerifyIP(package->options, sizeof(package->options), seed);
    return seedCheck;
}

void setData(PackageIP* packageIP, Package* package) {
    packageIP->package.checksum=package->checksum;
    packageIP->package.size = package->size;
    packageIP->package.destinationPort = package->destinationPort;
    packageIP->package.originPort = package->originPort;
    memcpy( packageIP->package.data,package->data , sizeof(package->data));
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