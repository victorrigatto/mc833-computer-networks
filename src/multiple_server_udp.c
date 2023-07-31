//Nome: Victor Rigatto
//RA: 178068

//SERVIDOR MULTI CLIENTE UDP

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

//Estrutura para os dados do arquivo de registros
struct data{
    char email[20];
    char nome[20];
    char sobrenome[20];
    char cidade[20];
    char curso[20];
    char habilidades[40];
    char experiencias[40];
};
struct data Data;

//Variáveis para uso em diversas funções
int sockfd, n;
socklen_t len;

//Estrutura padrão com dados para o socket servidor e cliente
struct sockaddr_in servaddr, cliaddr;

//Função para escrever os dados no arquivo de registros
void writeFile(struct data Data){
    FILE *file;
    file = fopen("data.dat","a");
    if(file){
        //Escreve dados no arquivo binário estrutura por estrutura
        fwrite (&Data, sizeof(struct data), 1, file);
    } else{
        printf("Nao pode abrir arquivo.\n");
    }
    fclose(file);
}

//Função para buscar os dados no arquivo de registros
void searchFile(char *str){
    FILE *file;
    file = fopen("data.dat","r");
    if(file){
        //Ler dados do arquivo binário estrutura por estrutura
        while(fread(&Data, sizeof(struct data), 1, file)){
            //Compara strings da busca desejada pelo cliente, se encontrar envia os dados para o cliente
            if(strcasecmp(Data.email,str) == 0 || strcasecmp(Data.curso,str) == 0 || strcasecmp(Data.habilidades,str) == 0){
                //Envia estrutura do registro para o cliente
                sendto(sockfd, &Data, sizeof(struct data), 0, (struct sockaddr*)&cliaddr, len);
            }
        }
    } else{
        printf("Nao pode abrir arquivo.\n");
    }
    fclose(file);
}

//Função para modificar registro no arquivo de registros
void changeFile(char *str, char *str2){
    FILE *file;
    file = fopen("data.dat","r");
    if(file){
        //Ler dados do arquivo binário estrutura por estrutura
        while(fread(&Data, sizeof(struct data), 1, file)){
            //Compara strings da busca desejada pelo cliente, se encontrar envia os dados para o cliente
            if(strcasecmp(Data.email,str) == 0 || strcasecmp(Data.curso,str) == 0 || strcasecmp(Data.habilidades,str) == 0){
                strcpy(Data.experiencias, str2);
                //Escreve nova estrutura no arquivo de registros
                writeFile(Data);
            }
        }
    } else{
        printf("Nao pode abrir arquivo.\n");
    }
    fclose(file);
}

//Função para enviar os dados do arquivo de registros
void printFile(char *str){
    //Variável de controle de fim de fluxo, 1 = parar
    int ctrl = 0;
    FILE *file;
    file = fopen("data.dat","r");
    if(file){
        //Ler dados do arquivo binário estrutura por estrutura
        while(fread(&Data, sizeof(struct data), 1, file)){
            //Compara strings da busca desejada pelo cliente, se encontrar envia os dados para o cliente
            if(strcasecmp(Data.email,str) == 0 || strcasecmp(Data.curso,str) == 0 || strcasecmp(Data.habilidades,str) == 0){
                //Envia variável de controle de fim de fluxo e estrutura do registro para o cliente
                sendto(sockfd, &ctrl, sizeof(ctrl), 0, (struct sockaddr*)&cliaddr, len);
                sendto(sockfd, &Data, sizeof(struct data), 0, (struct sockaddr*)&cliaddr, len);
            }
        }
        //Envia variável de controle de fim de fluxo
        ctrl = 1;
        sendto(sockfd, &ctrl, sizeof(ctrl), 0, (struct sockaddr*)&cliaddr, len);
    } else{
        printf("Nao pode abrir arquivo.\n");
    }
    fclose(file);
}

int main(){

    int func;
    char str[1000], str2[1000];

    //Limpa estrutura do socket
    bzero(&servaddr, sizeof(servaddr));
 
    //Socket de domínio de internet IPv4, UDP e protocolo padrão (0)
    //Cria socket para acesso à camada de transporte, retorna um descritor de socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 
    if (sockfd < 0){
        printf("Nao pode criar socket.\n");
        return 0;
    }

    //Especifica o domínio de internet IPv4 e recebe qualquer endereço na porta 5035
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(5035);
 
    //Aloca um "nome" (endereço local) para o socket
    //sockfd é o descritor do socket criado
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    while(1){

        len = sizeof(cliaddr);

        //Recebe função do cliente
        n = recvfrom(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&cliaddr, &len);
        
        switch(func){
            case 1:
                printf("Opção [1] recebida.\n");
                //Recebe estrutura do registro do cliente
                n = recvfrom(sockfd, &Data, sizeof(struct data), 0, (struct sockaddr*)&cliaddr, &len);
                //Executa função de registro em arquivo
                writeFile(Data);
                break;
            case 2:
                printf("Opção [2] recebida.\n");
                bzero(str, 1000);
                //Recebe string para busca
                n = recvfrom(sockfd, str, 1000, 0, (struct sockaddr*)&cliaddr, &len);
                //Recebe string para modificar
                n = recvfrom(sockfd, str2, 1000, 0, (struct sockaddr*)&cliaddr, &len);
                //Executa função de modificação de registro
                changeFile(str, str2);
                break;
            case 3:
                printf("Opção [3] recebida.\n");
                bzero(str, 1000);
                //Recebe string para busca
                n = recvfrom(sockfd, str, 1000, 0, (struct sockaddr*)&cliaddr, &len);
                //Executa função de busca no arquivo de registros
                searchFile(str);
                break;
            case 4:
                printf("Opção [4] recebida.\n");
                bzero(str, 1000);
                //Recebe string para busca
                n = recvfrom(sockfd, str, 1000, 0, (struct sockaddr*)&cliaddr, &len);
                //Executa função de leitura no arquivo de registros
                printFile(str);
                break;
            case 5:
                printf("Opção [5] recebida.\n");
                bzero(str, 1000);
                //Recebe string para busca
                n = recvfrom(sockfd, str, 1000, 0, (struct sockaddr*)&cliaddr, &len);
                //Executa função de leitura no arquivo de registros
                printFile(str);
                break;
        }
    }
}
