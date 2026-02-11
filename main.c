#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>


typedef struct CLIENTE{
    char nome[100];
    char telefone[100];
    char dataNascimento[100];
    char email[100];
    char CPF[100];
    struct CLIENTE *anterior;
    struct CLIENTE *proximo;
}CLIENTE;

typedef struct Lista{
    struct CLIENTE *inicio;
    struct CLIENTE *fim;
}Lista;

typedef struct DATA_NASCIMENTO{
    int dia;
    int mes;
    int ano;
    char data_completa[11];
}DATA_NASCIMENTO;

char* checar_idade(DATA_NASCIMENTO *data_nasc) {
    char confirmacao;
    int dias_no_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (sscanf(data_nasc->data_completa, "%d/%d/%d",&data_nasc->dia,&data_nasc->mes,&data_nasc->ano) != 3)
    {
        printf("\nFormato de data inválido. Use DD/MM/AAAA.\n");
        return NULL;
    }

    if (data_nasc->ano < 1900 || data_nasc->ano > 2100) {
        printf("\nAno inválido. Use um ano entre 1900 e 2100.\n");
        return NULL;
    }

    if (data_nasc->mes < 1 || data_nasc->mes > 12) {
        printf("\nMês inválido. Use um valor entre 1 e 12.\n");
        return NULL;
    }
    if ((data_nasc->ano % 4 == 0 && data_nasc->ano % 100 != 0) || (data_nasc->ano % 400 == 0))
        dias_no_mes[1] = 29;
    if (data_nasc->dia < 1 || data_nasc->dia > dias_no_mes[data_nasc->mes - 1]) {
        printf("\nDia inválido para o mês informado.\n");
        return NULL;
    }
    time_t agora = time(NULL);
    struct tm data_atual = *localtime(&agora);
    int ano_atual = data_atual.tm_year + 1900;
    int mes_atual = data_atual.tm_mon + 1;
    int dia_atual = data_atual.tm_mday;
    int idade = ano_atual - data_nasc->ano;
    if (mes_atual < data_nasc->mes || (mes_atual == data_nasc->mes && dia_atual < data_nasc->dia))
        idade--;
    if (idade >= 18)
           return data_nasc->data_completa;
    else
    {
        printf("\n--- ATENÇÃO ---\n");
        printf("O usuário possui %d anos (menor de idade).\n", idade);
        printf("Está acompanhado de um responsável legal ou possui autorização escrita? Apresente no balcão.\n");
        printf("Digite (s) para SIM ou (n) para NÃO: ");
        scanf(" %c", &confirmacao);
        if (confirmacao == 's' || confirmacao == 'S')
                return data_nasc->data_completa;
        else
            {
                printf("\nCadastro negado para menor desacompanhado.\n");
                return NULL;
            }
    }
    fflush(stdin);
}

Lista* Criar_Lista(){
    Lista *lista_cliente = (Lista*) malloc(sizeof(Lista));
    if(!lista_cliente)
       {
           printf("ERRO!!! Falha na alocação de memória\n");
            exit(1);
       }
    else
    {
        lista_cliente->inicio = NULL;
        lista_cliente->fim = NULL;
    }
    return lista_cliente;
}

void Exibir_Cliente(CLIENTE *cliente){
    printf("\n===============================================================================");
    printf("\n Nome: %s",cliente->nome);
    printf("\n CPF: %s",cliente->CPF);
    printf("\n Telefone: %s",cliente->telefone);
    printf("\n Data de Nascimento: %s",cliente->dataNascimento);
    printf("\n E-mail: %s",cliente->email);
    printf("\n Endereço na Memória: %p",(void*)cliente);
    printf("\n===============================================================================\n");
}


void Ler_Dados_Cliente(char *nome, char *dataNasc,char *email, char *telefone,char *CPF){
    printf("\n   CADASTRO DE NOVO CLIENTE \n");
    printf(" Nome completo: ");
    scanf(" %[^\n]",nome);
    printf(" CPF: ");
    scanf(" %[^\n]",CPF);
    printf(" Telefone: ");
    scanf(" %[^\n]",telefone);
    printf(" Data de Nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",dataNasc);
    printf(" E-mail: ");
    scanf(" %[^\n]",email);
    fflush(stdin);
}

int Lista_Vazia(Lista *lista_cliente){
    if(lista_cliente == NULL)
        return 1;
    if((lista_cliente->inicio) == NULL)
        return 1;
    else
        return 0;
}

CLIENTE* Alocar_Cliente(char *nome, char *dataNasc,char *email, char *telefone,char *CPF, CLIENTE *ant){
    CLIENTE *novo_cliente = (CLIENTE*) malloc(sizeof(CLIENTE));
    if(!novo_cliente)
        {
            printf("ERRO! Falha na alocação de memória\n");
            exit(1);
        }
    else
    {
        strcpy(novo_cliente->nome,nome);
        strcpy(novo_cliente->dataNascimento,dataNasc);
        strcpy(novo_cliente->CPF,CPF);
        strcpy(novo_cliente->email,email);
        strcpy(novo_cliente->telefone,telefone);
        novo_cliente->proximo = NULL;
        novo_cliente->anterior = ant;
        return novo_cliente;
    }
}

void Inserir_Cliente(Lista *lista_cliente){
    char nome[100],email[100],telefone[100],CPF[100],dataNasc[100];
    char *data_aprovada;
    Ler_Dados_Cliente(nome, dataNasc, email, telefone, CPF);
    DATA_NASCIMENTO data_temp;
    strncpy(data_temp.data_completa, dataNasc, 10);
    data_temp.data_completa[10] = '\0';
    data_aprovada = checar_idade(&data_temp);
    if (data_aprovada == NULL) {
        printf("\nCADASTRO ABORTADO. Verificação de idade falhou ou foi negada.\n");
        return;
    }
    CLIENTE *novo_cliente = Alocar_Cliente(nome,dataNasc,email,telefone,CPF,lista_cliente->fim);
    if(!novo_cliente)
        {
            printf("ERRO!Falha na alocação de memória\n");
            exit(1);
        }
    if(lista_cliente->fim == NULL)
        lista_cliente->inicio = novo_cliente;
    else
        lista_cliente->fim->proximo = novo_cliente;
    lista_cliente->fim = novo_cliente;

    printf("\n Cliente cadastrado com sucesso! \n");
}

CLIENTE* Buscar_Cliente_Por_Nome(Lista *lista_cliente, char *nomeBusca){
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia!\n");
        return NULL;
    }
    CLIENTE *aux = lista_cliente->inicio;
    while(aux != NULL)
    {
        if((stricmp(aux->nome, nomeBusca) == 0))
        {
            return aux;
        }
        aux = aux->proximo;
    }
    return NULL;
}

CLIENTE* Buscar_Cliente_Por_CPF(Lista *lista_cliente,char *CPFBusca){
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia!\n");
        return NULL;
    }
    CLIENTE *aux = lista_cliente->inicio;
    while(aux != NULL)
    {
        if((stricmp(aux->CPF, CPFBusca) == 0))
        {
            return aux;
        }
        aux = aux->proximo;
    }
    return NULL;
}

void Buscar_e_Exibir_Cliente_Por_CPF(Lista *lista_cliente){
    char CPFBusca[100];
    printf("\n=== BUSCAR CLIENTE POR CPF ===\n");
    printf(" Digite o CPF do cliente: ");
    scanf(" %[^\n]",CPFBusca);
    CLIENTE *encontrado = Buscar_Cliente_Por_CPF(lista_cliente, CPFBusca);
    if(encontrado != NULL)
    {
        printf("\nCliente encontrado!");
        Exibir_Cliente(encontrado);
    }
    else
    {
        printf("\nCliente não encontrado!\n");
    }
    fflush(stdin);
}

void Buscar_e_Exibir_Cliente_Por_Nome(Lista *lista_cliente){
    char nomeBusca[100];
    printf("\n BUSCAR CLIENTE \n");
    printf(" Digite o nome completo do cliente: ");
    scanf(" %[^\n]",nomeBusca);
    CLIENTE *encontrado = Buscar_Cliente_Por_Nome(lista_cliente, nomeBusca);
    if(encontrado != NULL)
    {
        printf("\nCliente encontrado!");
        Exibir_Cliente(encontrado);
    }
    else
    {
        printf("\nCliente não encontrado!\n");
        printf("Verifique se digitou o nome completo e tente novamente\n\n");
    }
    fflush(stdin);
}

int Remover_Cliente(Lista *lista_cliente){
    int opcao;
    char cliente_CPF[100],cliente_NOME[100];
    CLIENTE *cliente = NULL;

    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia!\n");
        return 0;
    }
    printf("\n    REMOVER CLIENTE \n");
    printf(" 1 - Remover por Nome\n");
    printf(" 2 - Remover por CPF\n");
    printf("Digite a opção desejada: ");
    scanf("%d",&opcao);
    if(opcao == 1)
        {
            printf(" Digite o nome do cliente : ");
            scanf(" %[^\n]",cliente_NOME);
            cliente = Buscar_Cliente_Por_Nome(lista_cliente,cliente_NOME);
        }
    else if(opcao == 2)
        {
            printf(" Digite o CPF do cliente : ");
            scanf(" %[^\n]",cliente_CPF);
            cliente = Buscar_Cliente_Por_CPF(lista_cliente, cliente_CPF);
        }
    else
        {
            printf("\n Opção inválida!\n");
            return 0;
        }
    if(cliente == NULL)
        {
            printf("\nCliente não encontrado!\n");
            return 0;
        }
    printf("\nCliente a ser removido: ");
    Exibir_Cliente(cliente);
    if(cliente->anterior != NULL)
        cliente->anterior->proximo = cliente->proximo;
    else
        lista_cliente->inicio = cliente->proximo;
    if(cliente->proximo != NULL)
        cliente->proximo->anterior = cliente->anterior;
    else
        lista_cliente->fim = cliente->anterior;
    CLIENTE *proximo = cliente->proximo;
    free(cliente);
    printf("\nCliente removido com sucesso!\n");
    fflush(stdin);
    return 1;
}

void Exibir_Lista(Lista *lista_cliente){
    int contador = 1;
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia! Nenhum cliente cadastrado.\n");
        return;
    }
    CLIENTE*aux = lista_cliente->inicio;
    printf("\n\n   LISTA DE CLIENTES DA BIBLIOTECA \n");
    printf(" Total de clientes: ");
    CLIENTE *temp = lista_cliente->inicio;
    int total = 0;
    while(temp != NULL)
    {
        total++;
        temp = temp->proximo;
    }
    printf("%d\n", total);
    while(aux != NULL)
    {
        printf("\n[Cliente %d]", contador);
        Exibir_Cliente(aux);
        aux = aux->proximo;
        contador++;
    }
}

void Navegar_Lista(Lista *lista_cliente){
    int opcao;
    int continuar = 1;
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia!\n");
        return;
    }
    CLIENTE *atual = lista_cliente->inicio;
    if(atual == NULL)
    {
        printf("\n Nenhum cliente para navegar!\n");
        return;
    }
    printf("\n=== NAVEGAÇÃO NA LISTA ===\n");
    printf("[Primeiro Cliente]\n");
    Exibir_Cliente(atual);
    do {
        printf("\nOPÇÕES DE NAVEGAÇÃO:\n");
        printf(" 0 - Voltar ao menu principal\n");
        printf(" 1 - Próximo cliente\n");
        printf(" 2 - Cliente anterior\n");
        printf("Escolha: ");
        scanf("%d",&opcao);
        switch(opcao){
            case 0:
                continuar = 0;
                break;
            case 1:
                if(atual->proximo != NULL)
                    {
                        atual = atual->proximo;
                        Exibir_Cliente(atual);
                    }
                else
                    {
                        printf("\nVocê está no último cliente da lista!\n");
                    }
                break;
            case 2:
                if(atual->anterior != NULL)
                    {
                        atual = atual->anterior;
                        Exibir_Cliente(atual);
                    }
                else
                    {
                        printf("\nVocê está no primeiro cliente da lista!\n");
                    }
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while(continuar);
}

void CSV(Lista *lista_cliente){
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia! Nada para salvar.\n");
        return;
    }
    FILE *arquivo = fopen("clientes_biblioteca.csv", "w");
    if(arquivo == NULL)
    {
        printf("\nErro ao criar arquivo!\n");
        return;
    }
    fprintf(arquivo,"Nome,CPF,Telefone,Data de Nascimento,E-mail\n");
    CLIENTE *aux = lista_cliente->inicio;
    while(aux != NULL)
    {
        fprintf(arquivo,"%s,%s,%s,%s,%s\n",aux->nome,aux->CPF,aux->telefone,aux->dataNascimento,aux->email);
        aux = aux->proximo;
    }
    fclose(arquivo);
    printf("\nLista salva com sucesso!\n");
    printf("Arquivo gerado:clientes_biblioteca.csv");
}

void TXT(Lista *lista_cliente){
    int cont = 1 ;
    if(Lista_Vazia(lista_cliente))
    {
        printf("\n Lista vazia! Nada para salvar.\n");
        return;
    }
    FILE *arquivo = fopen("clientes_biblioteca.txt", "w");
    if(arquivo == NULL)
    {
        printf("\nErro ao criar arquivo!\n");
        return;
    }
    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, "|   CADASTRO DE CLIENTES - BIBLIOTECA   |\n");
    fprintf(arquivo, "========================================\n\n");
    CLIENTE*aux = lista_cliente->inicio;
    while(aux != NULL)
    {
        fprintf(arquivo, "Cliente %d:\n",cont);
        fprintf(arquivo, "  Nome: %s\n", aux->nome);
        fprintf(arquivo, "  CPF: %s\n", aux->CPF);
        fprintf(arquivo, "  Telefone: %s\n", aux->telefone);
        fprintf(arquivo, "  Data de Nascimento: %s\n", aux->dataNascimento);
        fprintf(arquivo, "  E-mail: %s\n", aux->email);
        fprintf(arquivo, "----------------------------------------\n\n");
        aux = aux->proximo;
        cont++;
    }
    fclose(arquivo);
    printf("\nLista salva com sucesso!\n");
    printf("Arquivo gerado:clientes_biblioteca.csv");
}
void Salva_arquivos(Lista *lista_cliente){
    int opcao;

    printf("\n       SALVAR LISTA EM ARQUIVO   \n");
    printf("     ------------------------------\n ");
    printf("    |1 - Salvar em formato TXT   |\n");
    printf("     |2 - Salvar em formato CSV   |\n");
    printf("     |3 - Cancelar                |\n");
    printf("     ------------------------------\n");
    printf(" Digite a opção desejada: ");
    scanf("%d",&opcao);

    switch(opcao){
        case 1:
            TXT(lista_cliente);
            break;
        case 2:
          CSV(lista_cliente);
            break;
        case 3:
            printf("\n Você retornou ao menu inicial.\n");
            break;
        default:
            printf("\n Opção inválida!\n");
    }
}

void opcoes(Lista *lista_cliente,int escolha){
    switch(escolha){
        case 1:
            Inserir_Cliente(lista_cliente);
            break;
        case 2:
            Remover_Cliente(lista_cliente);
            break;
        case 3:
            Buscar_e_Exibir_Cliente_Por_Nome(lista_cliente);
            break;
        case 4:
            Buscar_e_Exibir_Cliente_Por_CPF(lista_cliente);
            break;
        case 5:
            Exibir_Lista(lista_cliente);
            break;
        case 6:
            Navegar_Lista(lista_cliente);
            break;
        case 7:
            Salva_arquivos(lista_cliente);
            break;
        default:
            if(escolha!=8)
                printf("\n Opção inválida!\n");
    }
}
int menu(){
    int opcao;
    printf("\n\n");
    printf("=====================================================\n");
    printf("|   SISTEMA DE CADASTRO - BIBLIOTECA                |\n");
    printf("=====================================================\n");
    printf("|  1 - Inserir Novo Cliente                         | \n");
    printf("|  2 - Remover Cliente                              |\n");
    printf("|  3 - Buscar Cliente por Nome                      |\n");
    printf("|  4 - Buscar cliente por CPF                       |\n");
    printf("|  5 - Exibir Lista de Clientes                     |\n");
    printf("|  6 - Navegar na Lista                             |\n");
    printf("|  7 - Salvar Lista em Arquivo                      |\n");
    printf("|  8 - Sair do Sistema                              |\n");
    printf("=====================================================\n");
    printf(" Digite a opção desejada: ");
    scanf("%d",&opcao);
    return opcao;
}

int main(){
    setlocale(LC_ALL,"Portuguese_Brazil");
    int opcao;
    Lista *lista_cliente = Criar_Lista();
    printf("\n");
    printf("============================================================\n");
    printf("|          B I B L I O T E C A   U E R J - Z O             |\n");
    printf("|         _______        _______        _______            |\n");
    printf("|        /      /,      /      /,      /      /,           |\n");
    printf("|       /      //      /      //      /      //            |\n");
    printf("|      /______//      /______//      /______//             |\n");
    printf("|     (______(/      (______(/      (______(/              |\n");
    printf("|                                                          |\n");
    printf("============================================================\n");
    do{
        opcao = menu();
        opcoes(lista_cliente,opcao);
    }while(opcao!=8);
    printf("\n Sistema encerrado. Até logo!\n\n");
    while(!Lista_Vazia(lista_cliente)){
        CLIENTE *temp = lista_cliente->inicio;
        lista_cliente->inicio = lista_cliente->inicio->proximo;
        free(temp);
    }
    free(lista_cliente);
    return 0;
}
