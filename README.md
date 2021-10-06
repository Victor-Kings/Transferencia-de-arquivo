
# Transferência de arquivo texto em C

## Descrição do projeto:

1. Implementação do pacote UDP utilizando a seguinte estrutura:


```c
typedef struct {
    char data[32];
    int16_t originPort;
    int16_t destinationPort;
    int16_t size;
    int16_t checksum;
}Package;
```
2. Função que preenche e consulta cada um dos campos do pacote.

```c
void populatePackage(Package pack, char data, int16_t originPort, int16_t destinationPort)

```

3. Função que cria um novo pacote.

```c
Package * createPackage()
```

4. Função que destroe o pacote.

```c
void deletePackage(Package *package)
```

5. Função que recebe, por parâmetro: o caminho para um arquivo (que neste caso será um txt) e também a uma quantidade máxima de bits a qual esse arquivo será fracionado. Essa função retorna um array ou uma estrutura de dados com o arquivo totalmente fragmentado conforme solicitado.

```c
void createFragments(char * file, int size, char fragments[1024][1024], int * numberOfFragments)
```

6. Criação de pacotes a serem transferidos, já com conteúdo completo. Usando os fragmentos gerados pela função anterior.

```c
void fragmentsToPackage(char fragments[1024][1024], int numberOfFragments, Package * packagelist, int *numberOfpackages)
```

7. Função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte será usado.
```c
void sendPackage(Package *packageList, char protocol, int numberOfpackages)
```

8. Função que valida se o pacote está construido corretamente.
```c
unsigned int checksumVerify(char *buffer, size_t len, unsigned int seed)
```

9. Função que faz a comunicação com um receptor.
```c
typedef struct {
    int sock;
    struct sockaddr_in name;
}socketType;

socketType socketCreate(char selectProtocol)
```

10. Programa que recebe os pacotes transmitidos
```
/server.c
```

11. Biblioteca com funções básicas.
```
/udpLib.h
```

12. Função que recebe os pacotes e os registra um a um em arquivo de texto para que possam ser analisados.
```c
receivePackage(char protocol, Package * packagelist, int *numberOfPackages)
```

13. Função que faz a análise dos pacotes recebidos.
```c
unsigned int checksumVerify(char *buffer, size_t len, unsigned int seed)
```

14. Função que remonta o arquivo recebido.
```c
void createFILE(Package * packageList, int numberOfPackages)
```

15. Função para imprimir na tela o conteúdo do arquivo de texto.
```c
void printPackage(Package * packageList, int numberOfPackages)
```

16. Implementação do pacote IP utilizando a seguinte estrutura:
```c
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
    int16_t headerCheckSum; 
    char sourceIpAddress[32];
    char destinationIp[32];
    char options[32];
    Package package;
}PackageIP;
```

17. Função para criar um novo pacote.
```c
PackageIP * createIPPackage()
```

18. Função para adicionar os IP's de origem e o de destino.
```c
void setIP(char* destinationIP, PackageIP* package)
```

19. Função para preencher o package no PackageIP.
```c
void setData(PackageIP* packageIP, Package* package)
```

20. Função generica para definir o checksum apenas do header.
```c
int getCheckSumHeader(PackageIP* package)
```

21. Função intermediaria para definir o checksum apenas do header.
```c
int void setHeaderCheckSum(PackageIP* package, unsigned int seed)
```

22. Função checksum para o header.
```c
unsigned int checksumVerifyIP(char *buffer, size_t len, unsigned int seed)
```

23. Função para definir tamanho do header.
```c
void setHeaderLength(PackageIP* package)
```

24. Função para definir identificação do header.
```c
void setIdentification(PackageIP* package, int index)
```

25. Função para definir tempo de limite de vida.
```c
void setTimeToLive(PackageIP* package)
```

26. Função para definir o offset do fragmento.
```c
void setFragmentOffset(PackageIP* package, int position )
```

27. Função para setar Flag, definindo se é fragmentado ou ultimo pacote.
```c
void setFlags(PackageIP* package,int fragmented, int lastFragment)
```

28. Função para setar opções, porem não há opções necessarias em nosso projeto.
```c
void setOptions(PackageIP* package)
```

29. Função para definir protocolo que sera utilizado.
```c
void setProtocol(PackageIP* package, char protocol)
```

30. Função para tamanho total, header+data.
```c
void setTotalLength(PackageIP* package)
```

31. Função para definir tipo de serviço, neste caso o mesmo que protocolo.
```c
void setServiceType(PackageIP* package, char param )
```

32. Função para definir a versão do http.
```c
void setVersion(PackageIP* package)
```

## Execute localmente com um terminal linux

Clone o projeto

```bash
  git clone https://github.com/Victor-Kings/Transferencia-de-arquivo.git
```

Vá para o diretório do projeto

```bash
  cd Transferencia-de-arquivo
```
Compile o server

```bash
  gcc server.c -o server
```
Compile o cliente

```bash
  gcc cli.c -o client
```

Execute o server
```bash
  ./server
```

Execute o cliente
```bash
  ./client
```

Brinque fazendo alterações no arquivo de texto :metal:

### Authors
- [Chistopher de Oliveira Souza](https://github.com/Christopher-OSouza)
- [Hiago da Silva](https://github.com/hiagomoa)
- [Leonardo Sanavio](https://github.com/LeoSanavio)
- [Murilo de Paula Araujo](https://www.github.com/murilodepa)
- [Victor Reis](https://github.com/Victor-Kings)
