#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdbool.h>
#include "ipLib.h"

#define splitFragment 50

void deletePackage(Package *package){
    free(package);
}

socketType socketCreate(char selectProtocol, char* ip){
    int sock;
    struct sockaddr_in name;
    struct hostent *hp, *gethostbyname();

    sock = socket (AF_INET, selectProtocol == 'U' ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (sock<0){
        perror ("Abertura de socket");
        exit (1);
    }
   
    name.sin_family = AF_INET;
    name.sin_addr.s_addr =  inet_addr(ip);
    name.sin_port= htons (1234);

    socketType socketUDP;
    socketUDP.sock= sock;
    socketUDP.name = name;
    return socketUDP;
}

void encapsulateIPPackage(Package *package, PackageIP *packageIP, char protocol, int numberOfPackages, int currentPackage,char* ip){
    int fragmented=0;
    int lastFragment=0;
    setVersion(packageIP);
    setServiceType(packageIP,protocol);
    setTotalLength(packageIP);
    setProtocol(packageIP,protocol);
    setOptions(packageIP);
    if(numberOfPackages>1)
    {
        fragmented=1;
    }

    if(currentPackage==numberOfPackages){
        lastFragment=1;
    }
    setFlags(packageIP, fragmented, lastFragment);
    setFragmentOffset(packageIP,currentPackage);
    setTimeToLive(packageIP);
    setIdentification(packageIP,currentPackage);
    setHeaderLength(packageIP);
    setIP(ip, packageIP);
    setHeaderCheckSum(packageIP,seed);
    setData(packageIP,package);
}

void sendPackageList(Package *packageList, socketType socket, int numberOfpackages, char protocol,char* ip){
    int i = 0;
    PackageIP* packageIP=createIPPackage();
    int lengthPackageIP = sizeof(*(packageIP));
    while(i < numberOfpackages) {
        encapsulateIPPackage(&packageList[i], packageIP, protocol, numberOfpackages, i, ip);
        int resp = sendto(socket.sock,(char*) packageIP, lengthPackageIP, 0, (struct sockaddr *)&socket.name, sizeof socket.name);
        if (resp < 0) printf("[ERRO] Envio do pacote %d/%d", i, lengthPackageIP);
        i++;
    }
}

bool validateChecksum(Package *packageList, int numberOfpackages){

    for(int i = 0; i < numberOfpackages; i++){
        int lengthPackage = strlen(packageList[i].data);
        unsigned int check = checksumVerify(packageList[i].data, lengthPackage, seed);
        if(check != packageList[i].checksum){
            return false;
        }
    }
    return true;
}

void createFragments(char * file, int size, char fragments[1024][1024], int * numberOfFragments){
    FILE *input_file;
    int n_bytes_read = 0;
    int i=0;
    input_file = fopen(file, "rb");

    while ((n_bytes_read = fread(fragments[i], 1, size, input_file)) > 0){
        i++;
	}
    *numberOfFragments=i;   
    fclose(input_file);
}
void populatePackage(Package *pack, char* data, int16_t originPort, int16_t destinationPort){
    memcpy(pack->data, data, 32);
    pack->originPort = originPort;
    pack->destinationPort = destinationPort;
    pack->size = strlen(pack->data);
}

void fragmentsToPackage(char fragments[1024][1024], int numberOfFragments, Package * packagelist, int *numberOfpackages)
{
    int sizeOfFragmentsAux;
    int j=0,aux=0, i=0;


    for(i=0; i<numberOfFragments; i++){
         sizeOfFragmentsAux = (strlen(fragments[i]));       
         j=0;

        while(sizeOfFragmentsAux > 0){
            Package * package = createPackage();
            char * fragAux= fragments[i] + j*32;
            populatePackage(package, fragAux, 1234, 1234);
            packagelist[aux] = *package;
            *numberOfpackages = *numberOfpackages + 1;
            if(sizeOfFragmentsAux >= 31) {
                packagelist[aux].data[32]='\0';
                packagelist[aux].checksum = checksumVerify(packagelist[aux].data, 32, 0);
            } else {
                packagelist[aux].data[sizeOfFragmentsAux]='\0';
                packagelist[aux].checksum = checksumVerify(packagelist[aux].data, sizeOfFragmentsAux, 0);
            }
            sizeOfFragmentsAux-=32;
            j++;
            aux++;
        }
    }
}
 
void sendInfoPackage(int numberPackage, socketType socketData){
    char number[10];
    sprintf(number, "%d", numberPackage);
    int resp = sendto(socketData.sock, number, sizeof(number), 0, (struct sockaddr *)&socketData.name, sizeof socketData.name);

    if (resp < 0) printf("[ERROR] envio do numeri de pacotes");
}

void sendPackage(Package *packageList, char protocol, int numberOfpackages, char* ip){
    socketType socketData = socketCreate(protocol,ip);
    sendInfoPackage(numberOfpackages, socketData);
    bool check = validateChecksum(packageList, numberOfpackages);
    if(check){
        sendPackageList(packageList, socketData, numberOfpackages, protocol,ip);
    }else{
        perror("Pacote errado");
    }
}

int main() {
    int sock = 0, i=0,numberOfFragments, numberOfpackages=0;
    Package * packageList =  malloc(1024*sizeof(Package));
    char fragments[1024][1024];
       
    createFragments("teste.txt", splitFragment, fragments, &numberOfFragments);

    fragmentsToPackage(fragments, numberOfFragments, packageList, &numberOfpackages);

    sendPackage(packageList, 'U', numberOfpackages,"127.0.0.1");

    return 0;
}