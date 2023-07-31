//Nome: Victor Rigatto
//RA: 178068

//CLIENTE

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

//Socket para uso em diversas funções
int sockfd;

//Função para receber separadamente os dados enviados pelo servidor
//Poderia utilizar serialização para receber a estrutura inteira de uma vez
void receiveFromServer(){
    char recvline[100];
    bzero(recvline, 100);
    read(sockfd, recvline, 100);
    printf("Email: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Nome: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Sobrenome: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Cidade: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Curso: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Habilidades: %s",recvline);
    read(sockfd, recvline, 100);
    printf("Experiencias: %s",recvline);
    printf("Pressione ENTER para voltar ao menu.\n");
    getchar();
}

void func(int sockfd, int func){

    //Buffer para enviar strings de caracteres para o servidor
    char sendline[100];

    //Limpa buffer e entrada do teclado
    bzero(sendline, 100);
    setbuf(stdin, NULL);

    //Seletor de funções do cliente
    switch(func){
        case 1:
            func = htonl(func);
            //Envia função para o servidor
            write(sockfd, &func, sizeof(func));
            //Envia informações do cliente uma a uma
            printf("Insira um email: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira um nome: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira um sobrenome: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira uma cidade: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira um curso: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira as habilidades: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira as experiencias: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            break;
        case 2:
            func = htonl(func);
            //Envia função para o servidor
            write(sockfd, &func, sizeof(func));
            printf("Insira um email: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            printf("Insira as experiencias: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            break;
        case 3:
            func = htonl(func);
            //Envia função para o servidor
            write(sockfd, &func, sizeof(func));
            printf("Insira um email: ");
            fgets(sendline, 100, stdin);
            //Envia string para o servidor
            write(sockfd, sendline, strlen(sendline)+1);
            //Função para receber strings uma a uma sincronizadamente
            receiveFromServer();
            break;
        case 4:
            func = htonl(func);
            //Envia função para o servidor
            write(sockfd, &func, sizeof(func));
            printf("Insira um curso: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            receiveFromServer();
            break;
        case 5:
            func = htonl(func);
            //Envia função para o servidor
            write(sockfd, &func, sizeof(func));
            printf("Insira uma habilidade: ");
            fgets(sendline, 100, stdin);
            write(sockfd, sendline, strlen(sendline)+1);
            receiveFromServer();
            break;
    }
}

int main(int argc, char **argv){

    int menu;

    //Estrutura padrão com dados para a conexão
    struct sockaddr_in servaddr;

    //Limpa estrutura
    bzero(&servaddr, sizeof(servaddr));

    //Socket de domínio de internet IPv4, TCP stream e protocolo padrão (0)
    //Cria socket para acesso à camada de transporte, retorna um descritor de socket
    sockfd=socket(AF_INET, SOCK_STREAM, 0);

    //Especifica o domínio de internet IPv4 e conexão na porta 22000
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
    //Especifica o endereço IPv4 em formato binário na estrutura
    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
 
    //Tenta conexão com o servidor especificado na estrutura utilizando o socket criado
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("Nao pode conectar.\n");
        return 0;
    }

    system("clear");

    //Driver de exibição do menu do cliente
    while(1){
        printf("[1] Cadastrar novo perfil.\n");
        printf("[2] Adicionar experiencia profissional.\n");
        printf("[3] Listar um perfil.\n");
        printf("[4] Listar todos os perfis por curso.\n");
        printf("[5] Listar todos os perfis por habilidade.\n");
        printf("[6] Sair.\n");
        printf("Escolha uma opcao: ");

        scanf("%d",&menu);

        switch(menu){
            case 1:
                func(sockfd, 1);
                break;
            case 2:
                func(sockfd, 2);
                break;
            case 3:
                func(sockfd, 3);
                break;
            case 4:
                func(sockfd, 4);
                break;
            case 5:
                func(sockfd, 5);
                break;
            case 6:
                close(sockfd); //Fechar
                return 0;
        }
    }
}
