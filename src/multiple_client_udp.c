//Nome: Victor Rigatto
//RA: 178068

//CLIENTE UDP

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>

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

//Estrutura padrão com dados para o socket servidor
struct sockaddr_in servaddr;

//Função de execução das opções do cliente
void func(int sockfd, int func){

    //Variável de controle de fim de fluxo, 1 = parar
    int ctrl = 0;

    //Buffer para enviar strings de caracteres para o servidor
    char sendline[1000];

    len = sizeof(servaddr);

    //Limpa buffer e entrada do teclado
    bzero(sendline, 1000);
    setbuf(stdin, NULL);

    //Seletor de funções do cliente
    switch(func){
        case 1:
            //Envia função para o servidor
            sendto(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&servaddr, len);
            //Preenche estrutura do registro
            printf("Insira um email: ");
            fgets(Data.email, 1000, stdin);
            printf("Insira um nome: ");
            fgets(Data.nome, 1000, stdin);
            printf("Insira um sobrenome: ");
            fgets(Data.sobrenome, 1000, stdin);
            printf("Insira uma cidade: ");
            fgets(Data.cidade, 1000, stdin);
            printf("Insira um curso: ");
            fgets(Data.curso, 1000, stdin);
            printf("Insira as habilidades: ");
            fgets(Data.habilidades, 1000, stdin);
            printf("Insira as experiencias: ");
            fgets(Data.experiencias, 100, stdin);
            //Envia estrutura do registro para o servidor
            sendto(sockfd, &Data, sizeof(struct data), 0, (struct sockaddr*)&servaddr, len);
            break;
        case 2:
            //Envia função para o servidor
            sendto(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&servaddr, len);
            printf("Insira um email: ");
            fgets(sendline, 1000, stdin);
            sendto(sockfd, sendline, 1000, 0, (struct sockaddr*)&servaddr, len);
            printf("Insira as experiencias: ");
            fgets(sendline, 1000, stdin);
            sendto(sockfd, sendline, 1000, 0, (struct sockaddr*)&servaddr, len);
            break;
        case 3:
            //Envia função para o servidor
            sendto(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&servaddr, len);
            printf("Insira um email: ");
            fgets(sendline, 1000, stdin);
            //Envia string para o servidor
            sendto(sockfd, sendline, 1000, 0, (struct sockaddr*)&servaddr, len);
            //Recebe estrutura do registro do servidor
            n = recvfrom(sockfd, &Data, sizeof(struct data), 0, NULL, NULL);
            printf("\n");
            printf("Email: %s", Data.email);
            printf("Nome: %s", Data.nome);
            printf("Sobrenome: %s", Data.sobrenome);
            printf("Cidade: %s", Data.cidade);
            printf("Curso: %s", Data.curso);
            printf("Habilidades: %s", Data.habilidades);
            printf("Experiencias: %s", Data.experiencias);
            break;
        case 4:
            //Envia função para o servidor
            sendto(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&servaddr, len);
            printf("Insira um curso: ");
            fgets(sendline, 1000, stdin);
            //Envia string para o servidor
            sendto(sockfd, sendline, 1000, 0, (struct sockaddr*)&servaddr, len);
            while(1){
                //Recebe variável de controle de fim de fluxo, 1 = parar
                n = recvfrom(sockfd, &ctrl, sizeof(ctrl), 0, NULL, NULL);
                if(ctrl){
                    return;
                }
                //Recebe estrutura do registro do servidor
                n = recvfrom(sockfd, &Data, sizeof(struct data), 0, NULL, NULL);
                printf("\n");
                printf("Email: %s", Data.email);
                printf("Nome: %s", Data.nome);
                printf("Sobrenome: %s", Data.sobrenome);
                printf("Cidade: %s", Data.cidade);
                printf("Curso: %s", Data.curso);
                printf("Habilidades: %s", Data.habilidades);
                printf("Experiencias: %s", Data.experiencias);
            }
            break;
        case 5:
            //Envia função para o servidor
            sendto(sockfd, &func, sizeof(func), 0, (struct sockaddr*)&servaddr, len);
            printf("Insira uma habilidade: ");
            fgets(sendline, 1000, stdin);
            //Envia string para o servidor
            sendto(sockfd, sendline, 1000, 0, (struct sockaddr*)&servaddr, len);
            while(1){
                //Recebe variável de controle de fim de fluxo, 1 = parar
                n = recvfrom(sockfd, &ctrl, sizeof(ctrl), 0, NULL, NULL);
                if(ctrl){
                    return;
                }
                //Recebe estrutura do registro do servidor
                n = recvfrom(sockfd, &Data, sizeof(struct data), 0, NULL, NULL);
                printf("\n");
                printf("Email: %s", Data.email);
                printf("Nome: %s", Data.nome);
                printf("Sobrenome: %s", Data.sobrenome);
                printf("Cidade: %s", Data.cidade);
                printf("Curso: %s", Data.curso);
                printf("Habilidades: %s", Data.habilidades);
                printf("Experiencias: %s", Data.experiencias);
            }
            break;
    }
}

int main(int argc, char **argv){

    int menu;

    //Limpa estrutura do socket
    bzero(&servaddr, sizeof(servaddr));

    //Socket de domínio de internet IPv4, UDP e protocolo padrão (0)
    //Cria socket para acesso à camada de transporte, retorna um descritor de socket
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0){
        printf("Nao pode criar socket.\n");
        return 0;
    }

    //Especifica o domínio de internet IPv4 e conexão na porta 5035
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(5035);

    //Conecta socket
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    system("clear");

    //Driver de exibição do menu do cliente
    while(1){
        printf("\n");
        printf("[1] Cadastrar novo perfil.\n");
        printf("[2] Adicionar experiencia profissional.\n");
        printf("[3] Listar um perfil.\n");
        printf("[4] Listar todos os perfis por curso.\n");
        printf("[5] Listar todos os perfis por habilidade.\n");
        printf("[6] Sair.\n");
        printf("Escolha uma opcao: ");
        printf("\n");

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
                close(sockfd);
                return 0;
        }
    }
}
