#include <stdio.h>
#include <string.h>
//#include <time.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>

// CONSTANTES - USO GERAL
#define ESPACO 32

#define MAX_MSG 76
#define MAX_STR 50

#define USERMOCK "pim"
#define PASSMOCK "Senha123"

#define ARQUIVO_COMORBIDADE "GrupoDeRisco_Comorbidade.csv"
#define ARQUIVO_PACIENTES   "CadastroDePacientes.csv"

// TIPOS PERSONALIZADOS
typedef struct data DATA;
struct data {
    int dia;
    int mes;
    int ano;
};

typedef struct endereco ENDERECO;
struct endereco {
    char rua[MAX_STR];
    char numero[6];
    char bairro[MAX_STR];
    char cidade[MAX_STR];
    char estado[MAX_STR];
    char cep[10];
};

typedef struct paciente PACIENTE;
struct paciente {
    char nome[MAX_STR];
    char cpf[12];
    char telefone[15];
    char email[MAX_STR];
    char dataNascimento[11];
    ENDERECO endereco;
    char dataDiagnostico[11];
    char comorbidade[13];
};

// DECLARACOES DAS FUNCOES
void gotoxy(int, int);
void novoTitulo(char[]);
void mensagemStatusBar(char[]);
void limpaMensagemStatusBar();
void imprimeSimbolo(int, int, int);
void imprimeTexto(int, int, char[]);
void desenhaLinha(int, int, int, int);
void desenhaColuna(int, int, int, int);
void desenhaTelaPrincipal();
void telaLogin();
void telaMenu();
void novoPaciente();
int salvaNovoPaciente(PACIENTE);
int salvaRiscoComorbidade(PACIENTE, int);
char* getComorbidade(char*);
int idadeEntreDatas(char*, char*);
void configuraApp();
char* campoString(int, int, char*, int, char*, char*, int);

// INICIO DO APP
int main() {
    configuraApp();

    telaLogin();

    telaMenu();

    return 0;
}

// FRONTEND
void gotoxy(int x, int y) {
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void imprimeTexto(int posX, int posY, char txt[]) {
    gotoxy(posX, posY);
    printf("%s", txt);
}

void imprimeSimbolo(int posX, int posY, int simbolo) {
    gotoxy(posX, posY);
    printf("%c", simbolo);
}

void mensagemStatusBar(char message[]) {
    desenhaLinha(2, 29, ESPACO, 78);
    imprimeTexto(3, 29, message);
    //return getch();
}

void desenhaTelaPrincipal() {
    system("cls");

    desenhaLinha(1, 1, 205, 80);
    desenhaLinha(1, 3, 205, 80);
    desenhaLinha(1, 28, 205, 80);
    desenhaLinha(1, 30, 205, 80);

    desenhaColuna(1, 1, 186, 30);
    desenhaColuna(80, 1, 186, 30);

    imprimeSimbolo(1, 1, 201);
    imprimeSimbolo(80, 1, 187);
    imprimeSimbolo(1, 3, 204);
    imprimeSimbolo(80, 3, 185);
    imprimeSimbolo(1, 28, 204);
    imprimeSimbolo(80, 28, 185);
    imprimeSimbolo(1, 30, 200);
    imprimeSimbolo(80, 30, 188);
}

void novoTitulo(char titulo[]) {
    /*
    char fixo[13] = "CovidSYS >> ";
    char strfinal[MAX_MSG];

    strcat(strcpy(strfinal, fixo), titulo);
    desenhaLinha(2, 2, 255, 78);
    */

    imprimeTexto(3, 2, "CovidSYS >>                    ");
    imprimeTexto(15, 2, titulo);
}

void desenhaLinha(int posicaoX, int posicaoY, int simbolo, int tamanho) {
    gotoxy(posicaoX, posicaoY);

    for (int i = 0; i < tamanho; i++) {
        printf("%c", simbolo);
    }
}

void desenhaColuna(int posicaoX, int posicaoY, int simbolo, int tamanho) {
    for (int i = posicaoY; i < (tamanho+posicaoY); i++) {
        gotoxy(posicaoX, i);
        printf("%c", simbolo);
    }
}

// FUNCIONALIDADES DO APP
void configuraApp() {
    system("MODE con cols=80 lines=30");

    system("title CovidSYS - PIM IV 2021.2");

    system("COLOR 16");

    system("cls");
}

void telaLogin() {
    char temp[12];
    char usuario[8];
    char senha[12];

    desenhaTelaPrincipal();
    novoTitulo("LOGIN");

    campoString(3, 5, "USUARIO", sizeof(usuario), "", temp, 1);
    strcpy(usuario, temp);

    campoString(3, 7, "SENHA", sizeof(senha), "", temp, 1);
    strcpy(senha, temp);

    if (strcmp(usuario, USERMOCK) != 0 || strcmp(senha, PASSMOCK) != 0) {
        mensagemStatusBar("DADOS DE LOGIN INVALIDOS! PRESSIONE QUALQUER TECLA PARA TENTAR NOVAMENTE...");
        getch();
        telaLogin();
    }
}

char* campoString(int posX, int posY, char* nomeCampo, int tamanho, char* formato, char* campo, int obrigatorio) {
    char msg[MAX_MSG];

    while (1) {
        snprintf(msg, sizeof(msg), "PREENCHA O CAMPO '%s'", nomeCampo);
        mensagemStatusBar(msg);

        desenhaLinha(posX, posY, ESPACO, 50);
        gotoxy(posX, posY);

        printf("%s%s: ", nomeCampo, formato);
        fflush(stdin);
        gets(campo);

        if (strlen(campo) == 0 && obrigatorio == 1) {
            snprintf(msg, sizeof(msg), "CAMPO '%s' DEVE SER INFORMADO.", nomeCampo);
        }
        else if (strlen(campo) > tamanho) {
            snprintf(msg, sizeof(msg), "CAMPO '%s' DEVE TER NO MAXIMO %d CARACTERES.", nomeCampo, tamanho);
        } else {
            break;
        }

        mensagemStatusBar(msg);
        getch();
    }

    return campo;
}

void telaMenu() {
    char op_menu;

    while (1) {
        desenhaTelaPrincipal();
        novoTitulo("MENU PRINCIPAL");

        imprimeTexto(3, 5, "1 - CADASTRAR PACIENTE");
        imprimeTexto(3, 7, "2 - PESQUISAR PACIENTE");
        imprimeTexto(3, 9, "0 - SAIR DO APP");
        mensagemStatusBar("ESCOLHA UMA DAS OPCOES ACIMA...");

        op_menu = getch();

        if (op_menu == '0') {
            return;
        }
        else if (op_menu == '1') {
            novoPaciente();
        }
        else {
            mensagemStatusBar("OPCAO DO MENU INVALIDA! PRESSIONE QUALQUER TECLA PARA TENTAR NOVAMENTE...");
        }

        getch();
    }
}

void novoPaciente() {
    PACIENTE novoPaciente;
    int idadeCalculada = 0;
    char temp[MAX_STR];

    desenhaTelaPrincipal();

    //---
    novoTitulo("CADASTRAR PACIENTE >> DADOS PESSOAIS");

    campoString(3, 5, "NOME", sizeof(novoPaciente.nome), "", temp, 1);
    strcpy(novoPaciente.nome, temp);
    
    campoString(3, 6, "CPF", sizeof(novoPaciente.cpf)-1, " (APENAS NUMEROS)", temp, 1);
    strcpy(novoPaciente.cpf, temp);
    
    campoString(3, 7, "DT.NASCTO", sizeof(novoPaciente.dataNascimento)-1, " (DD/MM/AAAA)", temp, 1);
    strcpy(novoPaciente.dataNascimento, temp);
    
    campoString(3, 8, "TELEFONE", sizeof(novoPaciente.telefone), "", temp, 1);
    strcpy(novoPaciente.telefone, temp);
    
    campoString(3, 9, "E-MAIL", sizeof(novoPaciente.email), "", temp, 1);
    strcpy(novoPaciente.email, temp);
    
    //---
    novoTitulo("CADASTRAR PACIENTE >> DADOS DO ENDERECO");

    campoString(3, 11, "LOGRADOURO", sizeof(novoPaciente.endereco.rua), "", temp, 1);
    strcpy(novoPaciente.endereco.rua, temp);
    
    campoString(3, 12, "NUMERO", sizeof(novoPaciente.endereco.numero), "", temp, 1);
    strcpy(novoPaciente.endereco.numero, temp);
    
    campoString(3, 13, "BAIRRO", sizeof(novoPaciente.endereco.bairro), "", temp, 1);
    strcpy(novoPaciente.endereco.bairro, temp);
    
    campoString(3, 14, "CIDADE", sizeof(novoPaciente.endereco.cidade), "", temp, 1);
    strcpy(novoPaciente.endereco.cidade, temp);
    
    campoString(3, 15, "ESTADO", sizeof(novoPaciente.endereco.estado), "", temp, 1);
    strcpy(novoPaciente.endereco.estado, temp);
    
    campoString(3, 16, "CEP", sizeof(novoPaciente.endereco.cep), " (99999-999)", temp, 1);
    strcpy(novoPaciente.endereco.cep, temp);
    
    //---
    novoTitulo("CADASTRAR PACIENTE >> DADOS DO DIAGNOSTICO");

    campoString(3, 18, "DT.DIAGNOSTICO", sizeof(novoPaciente.dataDiagnostico)-1, " (DD/MM/AAAA)", temp, 1);
    strcpy(novoPaciente.dataDiagnostico, temp);
    
    imprimeTexto(3, 19, "PACIENTE TEM COMORBIDADE?");
    imprimeTexto(3, 20, "[0]NAO [1]DIABETES [2]OBESIDADE [3]HIPERTENSAO [4]TUBERCULOSE [5]OUTROS");
    campoString(3, 21, "COMORBIDADE", sizeof(novoPaciente.comorbidade), "", temp, 1);
    strcpy(novoPaciente.comorbidade, getComorbidade(temp));
    
    //---
    idadeCalculada = idadeEntreDatas(novoPaciente.dataNascimento, novoPaciente.dataDiagnostico);

    if (strlen(novoPaciente.comorbidade) != 0 && idadeCalculada >= 65) {
        if (salvaRiscoComorbidade(novoPaciente, idadeCalculada) == 0) {
            mensagemStatusBar("GRUPO DE RISCO + COMORBIDADE! PRESSIONE QUALQUER TECLA PARA CONTINUAR...");
            getch();
        }
    }

    if (salvaNovoPaciente(novoPaciente) == 0)
        mensagemStatusBar("CADASTRADO FINALIZADO! PRESSIONE QUALQUER TECLA PARA VOLTAR AO MENU...");
    else
        mensagemStatusBar("ERRO AO CADASTRAR PACIENTE! PRESSIONE QUALQUER TECLA PARA VOLTAR AO MENU...");
}

char* getComorbidade(char* comorbidade) {
    if (strcmp(comorbidade, "1") == 0)
        return "DIABETES";
    else if (strcmp(comorbidade, "2") == 0)
        return "OBESIDADE";
    else if (strcmp(comorbidade, "3") == 0)
        return "HIPERTENSAO";
    else if (strcmp(comorbidade, "4") == 0)
        return "TUBERCULOSE";
    else if (strcmp(comorbidade, "5") == 0)
        return "OUTROS";
    else
        return "";
}

int idadeEntreDatas(char* dataNascimento, char* dataDiagnostico) {
    DATA dtNasc;
    DATA dtDiag;

    char *strDia = malloc(2);
    char *strMes = malloc(2);
    char *strAno = malloc(4);

    int totalNascto = 0;
    int totalDiagto = 0;

    // DATA DE NASCIMENTO
    dtNasc.dia = atoi(strncpy(strDia, dataNascimento, 2));
    free(strDia);

    dtNasc.mes = atoi(strncpy(strMes, (dataNascimento + 3), 2));
    free(strMes);

    dtNasc.ano = atoi(strncpy(strAno, (dataNascimento + 6), 4));
    free(strAno);

    // DATA DO DIAGNOSTICO
    dtDiag.dia = atoi(strncpy(strDia, dataDiagnostico, 2));
    free(strDia);

    dtDiag.mes = atoi(strncpy(strMes, (dataDiagnostico + 3), 2));
    free(strMes);

    dtDiag.ano = atoi(strncpy(strAno, (dataDiagnostico + 6), 4));
    free(strAno);

    // CALCULO E RETORNO
    totalNascto = ((365 * dtNasc.ano) + (30 * dtNasc.mes) + dtNasc.dia);
    totalDiagto = ((365 * dtDiag.ano) + (30 * dtDiag.mes) + dtDiag.dia);

    //printf("%d\n\n", ((totalDiagto - totalNascto) / 365));
    //getch();

    return ((totalDiagto - totalNascto) / 365);
}

int salvaRiscoComorbidade(PACIENTE paciente, int idadeCalculada) {
    FILE* arquivoComorbidade;
    arquivoComorbidade = fopen(ARQUIVO_COMORBIDADE, "aw");

    if (arquivoComorbidade == NULL) {
        printf("Problema ao abrir arquivo '%s'...", ARQUIVO_COMORBIDADE);
        printf("Verifique as permissões de escrita na pasta principal do programa.");
        getch();
        return 1;
    }

    fseek(arquivoComorbidade, 0, SEEK_END);
    if (ftell(arquivoComorbidade) == 0) {
        fprintf(arquivoComorbidade, "CEP,IDADE,COMORBIDADE\n", NULL);
    }

    fprintf(arquivoComorbidade, "%s,%d,%s\n", paciente.endereco.cep, idadeCalculada, paciente.comorbidade);
    fclose(arquivoComorbidade);

    return 0;
}

int salvaNovoPaciente(PACIENTE paciente) {
    FILE* arquivoPaciente;
    arquivoPaciente = fopen(ARQUIVO_PACIENTES, "aw");

    if (arquivoPaciente == NULL) {
        printf("Problema ao abrir arquivo '%s'...", ARQUIVO_PACIENTES);
        printf("Verifique as permissões de escrita na pasta principal do programa.");
        getch();

        return 1;
    }

    fseek(arquivoPaciente, 0, SEEK_END);
    if (ftell(arquivoPaciente) == 0) {
        fprintf(arquivoPaciente, "NOME,CPF,DATA_NASCIMENTO,TELEFONE,EMAIL,END_RUA,END_NUMERO,END_BAIRRO,END_CIDADE,END_ESTADO,END_CEP,DATA_DIAGNOSTICO,COMORBIDADE\n", NULL);
    }

    fprintf(arquivoPaciente, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            paciente.nome, paciente.cpf, paciente.dataNascimento, paciente.telefone, paciente.email,
            paciente.endereco.rua, paciente.endereco.numero, paciente.endereco.bairro,
            paciente.endereco.cidade, paciente.endereco.estado, paciente.endereco.cep,
            paciente.dataDiagnostico, paciente.comorbidade);

    fclose(arquivoPaciente);

    return 0;
}
