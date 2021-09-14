#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include "udpLib.h"

socketType socketCreate(char selectProtocol){
    int sock, len;
    struct sockaddr_in name;
    struct hostent *hp, *gethostbyname();

    sock = socket (AF_INET, selectProtocol == 'U' ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (sock<0){
        perror ("Abertura de socket");
        exit (1);
    }
   
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port= htons (1234);

    if(bind (sock, (struct sockaddr *)&name, sizeof name) < 0){
        perror ("binding datagrama");
        exit(1);
    }

    len = sizeof(name);
    if(getsockname(sock, (struct sockaddr *)&name, &len) < 0){
        perror("erro no getsockname");
        exit(1);
    }

    printf("Numero da porta atribuida: %d\n",ntohs(name.sin_port));

    socketType socketUDP;
    socketUDP.sock= sock;
    socketUDP.name = name;

    return socketUDP;
}

void receivePackage(char protocol, Package * packagelist, int *numberOfPackeges) {
    socketType socketData = socketCreate(protocol);
    Package * package = createPackage();
    int i=0, len=0;
    char number[10];
    
    recvfrom(socketData.sock, number, sizeof(number), 0, (struct sockaddr *)&socketData.name, &len);
    
    len = sizeof(socketData.name);
    *numberOfPackeges = atoi(number);
    
    while(i < *numberOfPackeges) {
        recvfrom(socketData.sock, (char *)&packagelist[i], sizeof(packagelist[i]), 0, (struct sockaddr *)&socketData.name, &len);
        unsigned int check = checksumVerify(packagelist[i].data,(strlen(packagelist[i].data)),seed);
        if(check != packagelist[i].checksum){
           printf("\n\n [ERROR] Pacote %d , esta incorreto, checksum recebido %d, check %d \n", i+1, packagelist[i].checksum, check);
        }
        i++;
    }
}

void createFILE(Package * packageList, int numberOfPackeges) {
  FILE *output_file;
  int i = 0;
  output_file = fopen("new_file.txt", "w");

  if (output_file == NULL)
    printf("\nError in file read!\n");

  while(i < numberOfPackeges) {
    fwrite(packageList[i].data, sizeof(char), strlen(packageList[i].data), output_file);
    i++;
  }
}

void printPackage(Package * packageList, int numberOfPackeges){
    for(int i=0;i<numberOfPackeges;i++){
        searchInfo(&packageList[i]);
    }
}

int main() {
    int numberOfPackeges;
    Package * packageList =  malloc(1024*sizeof(Package));
    receivePackage('U', packageList, &numberOfPackeges);
    createFILE(packageList, numberOfPackeges);
    printPackage(packageList, numberOfPackeges);
    exit(0);
}