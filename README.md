# README - Quebra de Senhas

# Como Rodar o Programa

## **1. Compilar**
```sh
gcc -o solution solution.c -O2 -fsanitize=address -g
```

## **2. Executar**
```sh
./solution passwords/ resultados/
```

## **3. Verificar Resultados**
```sh
ls resultados/
```
Os arquivos descriptografados estarão em `resultados/dec_XXX.txt`.

## **Exemplo de Saída**
```sh
Gerando 10 processos para processar arquivos...
Processo PID 1234: Quebrando senhas de passwords/001.txt
...
[Aguardando término da força bruta]
Processo PID 1234: Senhas quebradas salvas em resultados/dec_001.txt
...
[Fim dos processos descendentes]
Tempo total de execução: 700ms
```

## Universidade Federal da Paraíba - Centro de Informática
**Disciplina:** LPII - Programação Concorrente - 2024.2  
**Professor:** Carlos Eduardo Batista  
**Exercício Prático - Prova 1**  

## Descrição do Problema
Você trabalha na equipe de segurança de uma empresa que recentemente passou por um incidente: um ex-funcionário criptografou arquivos importantes utilizando um programa personalizado. O código fonte da função de criptografia foi encontrado, mas as chaves originais foram perdidas.

Seu objetivo é desenvolver um programa que utilize **força bruta** para descriptografar os arquivos e recuperar as senhas originais.

## Função de Criptografia
A função utilizada para criptografar as senhas é:

```c
#define NUM_LETRAS 26
#define ASCII_A 65
#define TAMANHO_SENHA 4
const int COPRIMES[4] = {3, 5, 7, 11};
const int SHIFTS[4] = {7, 11, 13, 17};

char* encrypt(const char* str) {
    char* str_result = (char*) malloc(sizeof(char) * (TAMANHO_SENHA + 1));
    if (!str_result) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TAMANHO_SENHA; i++) {
        char c = str[i];

        if (c >= 'A' && c <= 'Z') {
            int val = c - ASCII_A;
            val = (val * COPRIMES[i] + SHIFTS[i]) % NUM_LETRAS;
            str_result[i] = val + ASCII_A;
        } else {
            perror("Erro: String contém caracteres inválidos.");
            free(str_result);
            exit(EXIT_FAILURE);
        }
    }

    str_result[TAMANHO_SENHA] = '\0';
    return str_result;
}
```

## Requisitos
1. Desenvolver um programa em C para realizar um **ataque de força bruta**.
2. O programa deve recuperar as senhas originais a partir dos arquivos criptografados.
3. Todas as senhas têm exatamente **4 caracteres** e são compostas apenas por letras maiúsculas (**A-Z**).
4. O espaço de busca total é **26^4 = 456.976** combinações possíveis.

## Processo de Quebra de Senha
1. **Gerar Combinações**: Criar todas as combinações possíveis de senhas de 4 caracteres ("AAAA" até "ZZZZ").
2. **Aplicar Criptografia**: Para cada combinação, aplicar a função `encrypt()` e verificar se o resultado corresponde à senha criptografada.
3. **Verificação**: Identificar a senha original que gera a senha criptografada encontrada no arquivo.

## Implementação Concorrente
- Cada arquivo criptografado será processado por um **processo separado**.
- O processo original deverá aguardar o término de todos os processos descendentes.
- Ao final da execução, o tempo total de execução deverá ser exibido.

## Exemplo de Execução
Execução utilizando 10 processos para processar 10 arquivos criptografados:

```
$ ./quebra_senhas diretorio_senhas/
Gerando 10 processos para processar arquivos...
Processo PID 1234: Quebrando senhas de diretorio_senhas/passwd_00.txt
Processo PID 1235: Quebrando senhas de diretorio_senhas/passwd_01.txt
Processo PID 1236: Quebrando senhas de diretorio_senhas/passwd_02.txt
...
Processo PID 1243: Quebrando senhas de diretorio_senhas/passwd_10.txt
[Aguardando término da força bruta]
Processo PID 1234: Senhas quebradas salvas em diretorio_senhas/dec_passwd_00.txt
Processo PID 1235: Senhas quebradas salvas em diretorio_senhas/dec_passwd_01.txt
Processo PID 1236: Senhas quebradas salvas em diretorio_senhas/dec_passwd_02.txt
...
Processo PID 1243: Senhas quebradas salvas em diretorio_senhas/dec_passwd_10.txt
[Fim dos processos descendentes]
Tempo total de execução: 700ms
```

## Instruções de Entrega
- Enviar por e-mail para: **bidu @ ci . ufpb . br**
- O **título do e-mail** deve ser: `[[LPII-20242-E001]] NOME DO ALUNO - MATRÍCULA`
- O **arquivo de entrega** deve conter:
  - Todos os **códigos-fonte** em **C/C++** dentro de um diretório nomeado `MATRICULA_LPII-20242-E001`
  - O diretório deve ser comprimido em um arquivo ZIP: `MATRICULA_LPII-20242-E001.zip`
  - O arquivo ZIP deve conter um **arquivo de texto** com:
    - Instruções de compilação e execução.
    - Observações relevantes para correção.

## Penalidades
- **Não atender às instruções de entrega resultará na não correção do exercício.**
- **Trabalho individual** - plágio resultará na não correção do exercício.
- Valor: **até 3,0 pontos** para a primeira prova.

## Prazo
- **Entrega até:** 23h59 de **17/02/2025**.

