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
#include "udpLib.h"

#define splitFragment 50

void deletePackage(Package *package){
    free(package);
}

socketType socketCreate(char selectProtocol){
    int sock;
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

    hp = gethostbyname("localhost");
    if (hp==0){
        perror("Host nao encontrado");
        exit(2);
    }

    bcopy ((char*)hp->h_addr, (char *)&name.sin_addr, hp->h_length);

    printf("Numero da porta atribuida: %d\n",ntohs(name.sin_port));

    socketType socketUDP;
    socketUDP.sock= sock;
    socketUDP.name = name;
    return socketUDP;
}

void sendPackageList(Package *packageList, socketType socket, int numberOfpackeges){
    int lenghtPackageList = sizeof(packageList)/sizeof(packageList[0]), i = 0;

    while(i < numberOfpackeges) {
        int lenghtPackage = sizeof(packageList[i]);

        int resp = sendto(socket.sock, (char *)&packageList[i], lenghtPackage, 0, (struct sockaddr *)&socket.name, sizeof socket.name);

        if (resp < 0) printf("[ERRO] Envio do pacote %d/%d",i,lenghtPackage);

        i++;
    }
}

bool validateChecksum(Package *packageList, int numberOfpackeges){

    for(int i = 0; i < numberOfpackeges; i++){
        int lenghtPackage = strlen(packageList[i].data);
        unsigned int check = checksumVerify(packageList[i].data, lenghtPackage, seed);
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

void fragmentsToPackage(char fragments[1024][1024], int numberOfFragments, Package * packagelist, int *numberOfpackeges)
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
            *numberOfpackeges = *numberOfpackeges + 1;
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
 
void sendInfoPackege(int numberPackage, socketType socketData){
    char number[10];
    sprintf(number, "%d", numberPackage);
    int resp = sendto(socketData.sock,number, sizeof(number), 0, (struct sockaddr *)&socketData.name, sizeof socketData.name);

    if (resp < 0) printf("[ERROR] envio do numeri de pacotes");
}

void sendPackage(Package *packageList, char protocol, int numberOfpackeges){
    socketType socketData = socketCreate(protocol);

    sendInfoPackege(numberOfpackeges, socketData);

    bool check = validateChecksum(packageList, numberOfpackeges);
    if(check){
        sendPackageList(packageList, socketData, numberOfpackeges);
    }else{
        perror("Pacote errado");
    }
}

int main() {
    int sock = 0, i=0,numberOfFragments, numberOfpackeges=0;
    Package * packageList =  malloc(1024*sizeof(Package));
    PackageIP *packPezinho = createIPPackage();
    char fragments[1024][1024];
       
    createFragments("teste.txt", splitFragment, fragments, &numberOfFragments);

    fragmentsToPackage(fragments, numberOfFragments, packageList, &numberOfpackeges);

    setIP("123.3123.123", packPezinho);
    printf("\nORIGEM: %s\n",packPezinho->sourceIpAddress);
    printf("\nDESTINO: %s\n",packPezinho->destinationIp);

    sendPackage(packageList, 'U', numberOfpackeges);


    return 0;
}