//Nome: Victor Rigatto
//RA: 178068

//SERVIDOR

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

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
int comm_fd;

//Função para enviar separadamente os dados requisitados pelo cliente
//Poderia utilizar serialização para enviar a estrutura inteira de uma vez
void sendToClient(struct data Data){
    write(comm_fd, Data.email, 100);
    write(comm_fd, Data.nome, 100);
    write(comm_fd, Data.sobrenome, 100);
    write(comm_fd, Data.cidade, 100);
    write(comm_fd, Data.curso, 100);
    write(comm_fd, Data.habilidades, 100);
    write(comm_fd, Data.experiencias, 100);
}

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
        while(fread(&Data, sizeof(struct data), 1, file))
        //Compara strings da busca desejada pelo cliente
        //Se encontrar, envia os dados para o cliente
        if(strcasecmp(Data.email,str) == 0 || strcasecmp(Data.curso,str) == 0 || strcasecmp(Data.habilidades,str) == 0){
            sendToClient(Data);
        }
    } else{
        printf("Nao pode abrir arquivo.\n");
    }
    fclose(file);
}

int main(){
    
    int listen_fd, func;
    char str[100];
 
    //Estrutura padrão com dados para a conexão
    struct sockaddr_in servaddr;

    //Limpa estrutura
    bzero(&servaddr, sizeof(servaddr));
 
    //Socket de domínio de internet IPv4, TCP stream e protocolo padrão (0)
    //Cria socket para acesso à camada de transporte, retorna um descritor de socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    //Especifica o domínio de internet IPv4 e recebe qualquer endereço na porta 22000
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);
 
    //Aloca um "nome" (endereço local) para o socket
    //listen_fd é o descritor do socket criado
    //Recebe endereço do cliente
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    //Anuncia a disponibilidade de aceitar conexões, informa o tamanho da fila
    listen(listen_fd, 10);
 
    //Aceita conexão e retorna um socket criado pelo SO para o cliente que conectou
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
 
    while(1){

        //Recebe função escolhida pelo cliente
        read(comm_fd, &func, sizeof(func));

        switch(ntohl(func)){
            case 1:
                printf("Opção [1] recebida.\n");
                read(comm_fd, Data.email, 100);
                read(comm_fd, Data.nome, 100);
                read(comm_fd, Data.sobrenome, 100);
                read(comm_fd, Data.cidade, 100);
                read(comm_fd, Data.curso, 100);
                read(comm_fd, Data.habilidades, 100);
                read(comm_fd, Data.experiencias, 100);
                writeFile(Data);
                break;
            case 2:
                printf("Opção [2] recebida.\n");
                bzero(str, 100);
                read(comm_fd, str, 100);
                read(comm_fd, str, 100);
                break;
            case 3:
                printf("Opção [3] recebida.\n");
                bzero(str, 100);
                read(comm_fd, str, 100);
                searchFile(str);
                break;
            case 4:
                printf("Opção [4] recebida.\n");
                bzero(str, 100);
                read(comm_fd, str, 100);
                searchFile(str);
                break;
            case 5:
                printf("Opção [5] recebida.\n");
                bzero(str, 100);
                read(comm_fd, str, 100);
                searchFile(str);
                break;
        }
    }
}
