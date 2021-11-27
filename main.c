#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>

// CONSTANTES - USO GERAL
#define ESPACO 32

#define MAX_MSG 76
#define MAX_STR 50

#define CABECALHO "NOME,CPF,DATA_NASCIMENTO,TELEFONE,EMAIL,END_RUA,END_NUMERO,END_BAIRRO,END_CIDADE,END_ESTADO,END_CEP,DATA_DIAGNOSTICO,COMORBIDADE,IDADE\n"
#define BUFFER_SIZE 1024

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
    char rua[50];
    char numero[6];
    char bairro[21];
    char cidade[21];
    char estado[3];
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
    int  idade;
};

// DECLARACOES DAS FUNCOES
void gotoxy(int, int);
void novoTitulo(char[]);
void mensagemStatusBar(char[]);
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
void pesquisarPaciente();
void imprimirPaciente(PACIENTE, int, int);
int buscarNoArquivo(char*);
void imprimePaciente(PACIENTE);


// INICIO DO APP
int main() {
    configuraApp();

    //telaLogin();

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

        if (strlen(campo) == 0 && campo[0] == '\0' && obrigatorio == 1) {
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
        else if (op_menu == '2') {
            pesquisarPaciente();
        }
        else {
            mensagemStatusBar("OPCAO DO MENU INVALIDA! PRESSIONE QUALQUER TECLA PARA TENTAR NOVAMENTE...");
        }

        getch();
    }
}

void novoPaciente() {
    PACIENTE novoPaciente;
    char temp[MAX_STR];

    desenhaTelaPrincipal();

    //---
    novoTitulo("CADASTRAR PACIENTE >> DADOS PESSOAIS");
    imprimeTexto(3, 27, "* = CAMPO OBRIGATORIO.");


    campoString(3, 5, "NOME", sizeof(novoPaciente.nome)-1, "*", temp, 1);
    strcpy(novoPaciente.nome, temp);

    campoString(3, 6, "CPF", sizeof(novoPaciente.cpf)-1, " (APENAS NUMEROS)*", temp, 1);
    strcpy(novoPaciente.cpf, temp);

    campoString(3, 7, "DT.NASCTO", sizeof(novoPaciente.dataNascimento)-1, " (DD/MM/AAAA)*", temp, 1);
    strcpy(novoPaciente.dataNascimento, temp);

    campoString(3, 8, "TELEFONE", sizeof(novoPaciente.telefone), "*", temp, 1);
    strcpy(novoPaciente.telefone, temp);

    campoString(3, 9, "E-MAIL", sizeof(novoPaciente.email), "*", temp, 1);
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

    campoString(3, 16, "CEP", sizeof(novoPaciente.endereco.cep), " (99999-999)*", temp, 1);
    strcpy(novoPaciente.endereco.cep, temp);

    //---
    novoTitulo("CADASTRAR PACIENTE >> DADOS DO DIAGNOSTICO");

    campoString(3, 18, "DT.DIAGNOSTICO", sizeof(novoPaciente.dataDiagnostico)-1, " (DD/MM/AAAA)*", temp, 1);
    strcpy(novoPaciente.dataDiagnostico, temp);

    imprimeTexto(3, 19, "PACIENTE TEM COMORBIDADE?");
    imprimeTexto(3, 20, "[0]NAO [1]DIABETES [2]OBESIDADE [3]HIPERTENSAO [4]TUBERCULOSE [5]OUTROS");
    campoString(3, 21, "COMORBIDADE", sizeof(novoPaciente.comorbidade), "*", temp, 1);
    strcpy(novoPaciente.comorbidade, getComorbidade(temp));

    //---
    novoPaciente.idade = idadeEntreDatas(novoPaciente.dataNascimento, novoPaciente.dataDiagnostico);

    if (strlen(novoPaciente.comorbidade) != 0 && novoPaciente.idade >= 65) {
        if (salvaRiscoComorbidade(novoPaciente, novoPaciente.idade) == 0) {
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

    // DATA DE NASCIMENTO
    dtNasc.dia = atoi(strncpy(strDia, dataNascimento, 2));
    dtNasc.mes = atoi(strncpy(strMes, (dataNascimento + 3), 2));
    dtNasc.ano = atoi(strncpy(strAno, (dataNascimento + 6), 4));

    // DATA DO DIAGNOSTICO
    dtDiag.dia = atoi(strncpy(strDia, dataDiagnostico, 2));
    dtDiag.mes = atoi(strncpy(strMes, (dataDiagnostico + 3), 2));
    dtDiag.ano = atoi(strncpy(strAno, (dataDiagnostico + 6), 4));

    // CALCULO E RETORNO
    int idade = dtDiag.ano - dtNasc.ano;
    if ((dtDiag.mes * 100 + dtDiag.dia) < (dtNasc.mes * 100 + dtNasc.dia))
        idade--;

    return idade;
}

int salvaRiscoComorbidade(PACIENTE paciente, int idadeCalculada) {
    FILE* arquivoComorbidade;
    arquivoComorbidade = fopen(ARQUIVO_COMORBIDADE, "aw");

    if (arquivoComorbidade == NULL) {
        printf("Problema ao abrir arquivo '%s'...", ARQUIVO_COMORBIDADE);
        printf("Verifique as permissoes de escrita na pasta principal do programa.");
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
        printf("Verifique as permissoes de escrita na pasta principal do programa.");
        getch();

        return 1;
    }

    fseek(arquivoPaciente, 0, SEEK_END);
    if (ftell(arquivoPaciente) == 0) {
        fprintf(arquivoPaciente, CABECALHO, NULL);
    }

    fprintf(arquivoPaciente, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d\n",
            paciente.nome, paciente.cpf, paciente.dataNascimento, paciente.telefone, paciente.email,
            paciente.endereco.rua, paciente.endereco.numero, paciente.endereco.bairro,
            paciente.endereco.cidade, paciente.endereco.estado, paciente.endereco.cep,
            paciente.dataDiagnostico, paciente.comorbidade, paciente.idade);

    fclose(arquivoPaciente);

    return 0;
}

void pesquisarPaciente() {
    char temp[MAX_STR];
    char cpf[12];

    desenhaTelaPrincipal();

    novoTitulo("PESQUISAR PACIENTE");

    campoString(3, 5, "CPF", sizeof(cpf)-1, "", temp, 1);
    strcpy(cpf, temp);

    buscarNoArquivo(cpf);

    getch();
}

void imprimePaciente(PACIENTE p) {
    gotoxy(3, 8);
    printf("NOME: %s\n", p.nome);

    //NÃO PRECISA MOSTRAR O CPF, JÁ FOI O CAMPO PESQUISADO
    //gotoxy(3, 10);
    //printf("CPF: %s", p.cpf);

    gotoxy(3, 10);
    printf("DT.NASCTO: %s", p.dataNascimento);

    gotoxy(3, 12);
    printf("E-MAIL: %s", p.email);

    gotoxy(3, 14);
    printf("ENDERECO: ");
    if (strlen(p.endereco.rua) > 0)
        printf(p.endereco.rua);

    if (strlen(p.endereco.numero) > 0) {
        if (strlen(p.endereco.rua) > 0)
            printf(", ");
        printf(p.endereco.numero);
    }

    gotoxy(3, 15);
    printf("          ");
    if (strlen(p.endereco.bairro) > 0) {
        printf(p.endereco.bairro);
    }

    if (strlen(p.endereco.cidade) > 0) {
        if (strlen(p.endereco.bairro) > 0)
            printf(", ");
        printf(p.endereco.cidade);
    }

    if (strlen(p.endereco.estado) > 0) {
        if (strlen(p.endereco.cidade) > 0)
            printf(", ");
        printf(p.endereco.estado);
    }
    //--FIM ENDERECO

    gotoxy(3, 17);
    printf("CEP: %s", p.endereco.cep);

    gotoxy(3, 19);
    printf("DT.DIAGNOSTICO: %s", p.dataDiagnostico);

    gotoxy(3, 21);
    printf("COMORBIDADE: %s", p.comorbidade);

    gotoxy(3, 23);
    printf("IDADE: %d", p.idade);
}

const char* getfield(char* line, int num) {
	const char* tok;
	for (tok = strtok(line, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n")) {
		if (!--num)
			return tok;
	}
	return NULL;
}

//int buscarNoArquivo(const char* nome, const char* cpf, int lista) {
int buscarNoArquivo(char* cpf) {
    FILE* arquivoPaciente;
    arquivoPaciente = fopen(ARQUIVO_PACIENTES, "r");

    char line[BUFFER_SIZE];
    int encontrado = 0;

    novoTitulo("PESQUISAR PACIENTE >> RESULTADO");

    while (fgets(line, BUFFER_SIZE, arquivoPaciente)) {
        char* tmp = strdup(line);

        if (strcmp(CABECALHO, tmp) != 0) {
            PACIENTE paciente;

            char dados[14][MAX_STR];
            int i = 0;
            const char* tok;

            for (tok = strtok(tmp, ","); tok && *tok; i++, tok = strtok(NULL, ","))
            {

                strcpy(dados[i], tok);
            }

            if (strcmp(dados[1], cpf) == 0)
            {
                gotoxy(3, 8);
                printf("NOME: %s\n", dados[0]);

                //NÃO PRECISA MOSTRAR O CPF, JÁ FOI O CAMPO PESQUISADO
                //gotoxy(3, 10);
                //printf("CPF: %s", p.cpf);

                gotoxy(3, 10);
                printf("DT.NASCTO: %s", dados[2]);

                gotoxy(3, 12);
                printf("TELEFONE: %s", dados[3]);

                gotoxy(3, 14);
                printf("E-MAIL: %s", dados[4]);

                gotoxy(3, 16);
                printf("ENDERECO: >> ");
                printf("%d", strlen(dados[5]));
                if (strlen(dados[5]) > 0)
                    printf(dados[5]);

                printf("%d", strlen(dados[6]));
                if (strlen(dados[6]) > 0) {
                    if (strlen(dados[5]) > 0)
                        printf(", ");
                    printf(dados[6]);
                }

                gotoxy(3, 17);
                printf("          >> ");
                if (strlen(dados[7]) > 0) {
                    printf(dados[7]);
                }

                if (strlen(dados[8]) > 0) {
                    if (strlen(dados[7]) > 0)
                        printf(", ");
                    printf(dados[8]);
                }

                if (strlen(dados[9]) > 0) {
                    if (strlen(dados[8]) > 0)
                        printf(", ");
                    printf(dados[9]);
                }
                //--FIM ENDERECO

                gotoxy(3, 19);
                printf("CEP: %s", dados[10]);

                gotoxy(3, 21);
                printf("DT.DIAGNOSTICO: %s", dados[11]);

                gotoxy(3, 23);
                printf("COMORBIDADE: %s", dados[12]);

                gotoxy(3, 25);
                printf("IDADE: %s", dados[13]);












                    /*
                strcpy(paciente.nome, dados[0]);
                strcpy(paciente.cpf, dados[1]);
                strcpy(paciente.dataNascimento, dados[2]);
                strcpy(paciente.telefone, dados[3]);
                strcpy(paciente.email, dados[4]);
                strcpy(paciente.endereco.rua, dados[5]);
                strcpy(paciente.endereco.numero, dados[6]);
                strcpy(paciente.endereco.bairro, dados[7]);
                strcpy(paciente.endereco.cidade, dados[8]);
                strcpy(paciente.endereco.estado, dados[9]);
                strcpy(paciente.endereco.cep, dados[10]);
                strcpy(paciente.dataDiagnostico, dados[11]);
                strcpy(paciente.comorbidade, dados[12]);
                paciente.idade = atoi(dados[13]);

                imprimePaciente(paciente);
                */
                encontrado = 1;

                break;
            }
        }

        memset(line, 0, BUFFER_SIZE);
        free(tmp);
    }

    fclose(arquivoPaciente);

    if (!encontrado)
        mensagemStatusBar("PACIENTE NÃO ENCONTRADO! PRESSIONE QUALQUER TECLA PARA CONTINUAR...");

    mensagemStatusBar("PRESSIONE [N] PARA NOVA PESQUISA OU QUALQUER TECLA PARA RETORNAR AO MENU...");
}

/**
 * Procura a string b na string a.
 *
 * Retorna -1 se `b` n�o for encontrada em `a`, ou o �ndice da
 * primeira ocorr�ncia de b em a.
 */
int strpos(const char *a, const char *b, int ignoreCase)
{
    int alen, blen;    /* comprimento das strings */
    int found_at;      /* vari�vel auxiliar: guardar� a posi��o da
                           primeira ocorr�ncia de b em a */
    int i, j;          /* vari�veis auxiliares usadas nos loops */

    /* calcular o comprimento de cada string */
    alen = strlen(a);
    blen = strlen(b);

    /* se b � mais longa que a, com certeza a n�o cont�m b */
    if (blen > alen) {
        return -1;
    }

    /* este loop externo encontra a primeira ocorr�ncia de b[0] em a */
    found_at = -1;
    for (i = 0; i < alen; ++i) {
        if (ignoreCase ? toupper(a[i]) == toupper(b[0]) : a[i] == b[0]) {

            /* guarda a posi��o da ocorr�ncia */
            found_at = i;

            /* este loop interno compara a substring `a[i:i+j]` byte-a-byte
             * com b */
            for (j = 0; j < blen && found_at != -1; ++j) {
                if (ignoreCase ? toupper(a[i + j]) != toupper(b[j]) : a[i + j] == b[j]) {
                    /* se um byte n�o for igual, ent�o � por que n�o encontramos */
                    found_at = -1;
                }
            }

            /* encontrado, retorna a posi��o guardada em found_at */
            /* aqui a gente poderia simplesmente usar break, mas para tornar o
             * c�digo ainda mais "simples" vamos retornar */
            if (found_at != -1) {
                return i;
            }
        }
    }

    return found_at;
}
