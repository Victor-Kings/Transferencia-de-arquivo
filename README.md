
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
void fragmentsToPackage(char fragments[1024][1024], int numberOfFragments, Package * packagelist, int *numberOfpackeges)
```

7. Função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte será usado.
```c
void sendPackage(Package *packageList, char protocol, int numberOfpackeges)
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
receivePackage(char protocol, Package * packagelist, int *numberOfPackeges)
```

13. Função que faz a análise dos pacotes recebidos.
```c
unsigned int checksumVerify(char *buffer, size_t len, unsigned int seed)
```

14. Função que remonta o arquivo recebido.
```c
void createFILE(Package * packageList, int numberOfPackeges)
```

15. Função para imprimir na tela o conteúdo do arquivo de texto.
```c
void printPackage(Package * packageList, int numberOfPackeges)
```


## Rode localmente com um terminal linux

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

### Developers 
* Hiago da Silva
* Chistopher de Oliveira Souza.  
* Murilo de Paula Araujo.
* Victor Reis
* Leonardo Sanavio
