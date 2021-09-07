int seed = 0;

typedef struct {
    char data[32];
    int16_t originPort;
    int16_t destinationPort;
    int16_t size;
    int16_t checksum;
}Package;

typedef struct {
    int sock;
    struct sockaddr_in name;
}socketType;

unsigned int checksumVerify(char *buffer, size_t len, unsigned int seed){
    unsigned char *buf = (unsigned char *)buffer;
    size_t i;
    for (i = 0; i < len; ++i)
    seed += (unsigned int)(*buf++);
    return seed;
}

Package * createPackage() {
    Package * package = malloc(sizeof(Package));
    return package;
}

void searchInfo(Package *pack){
    printf("\nDATA: %s", pack->data);
    printf("\nOrigin: %u", pack->originPort);
    printf("\nDestination: %u", pack->destinationPort);
    printf("\nSize: %u", pack->size);
    printf("\nChecksum: %u\n", pack->checksum);
}