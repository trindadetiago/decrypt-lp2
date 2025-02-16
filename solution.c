#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define NUM_LETRAS 26
#define ASCII_A 65
#define TAMANHO_SENHA 4
const int COPRIMES[4] = {3, 5, 7, 11};
const int SHIFTS[4] = {7, 11, 13, 17};

char *encrypt(const char *str)
{
    char *str_result = (char *)malloc(sizeof(char) * (TAMANHO_SENHA + 1));
    if (!str_result)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TAMANHO_SENHA; i++)
    {
        char c = str[i];
        if (c >= 'A' && c <= 'Z')
        {
            int val = c - ASCII_A;
            val = (val * COPRIMES[i] + SHIFTS[i]) % NUM_LETRAS;
            str_result[i] = val + ASCII_A;
        }
        else
        {
            perror("Erro: String contém caracteres inválidos.");
            free(str_result);
            exit(EXIT_FAILURE);
        }
    }

    str_result[TAMANHO_SENHA] = '\0';
    return str_result;
}

// Função que tenta descriptografar a senha original via força bruta
void brute_force(const char *encrypted, char *result)
{
    char attempt[TAMANHO_SENHA + 1];
    for (char a = 'A'; a <= 'Z'; a++)
    {
        for (char b = 'A'; b <= 'Z'; b++)
        {
            for (char c = 'A'; c <= 'Z'; c++)
            {
                for (char d = 'A'; d <= 'Z'; d++)
                {
                    attempt[0] = a;
                    attempt[1] = b;
                    attempt[2] = c;
                    attempt[3] = d;
                    attempt[4] = '\0';

                    char *enc_attempt = encrypt(attempt);
                    if (strcmp(enc_attempt, encrypted) == 0)
                    {
                        strcpy(result, attempt);
                        free(enc_attempt);
                        return;
                    }
                    free(enc_attempt);
                }
            }
        }
    }
}

// Função para processar um arquivo de entrada e descriptografar as senhas
void process_file(const char *filename, const char *output_dir)
{
    printf("Processo PID %d: Quebrando senhas de %s\n", getpid(), filename);
    fflush(stdout);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    mkdir(output_dir, 0777); // Garante que o diretório de saída exista

    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s/dec_%s", output_dir, strrchr(filename, '/') + 1);

    FILE *output = fopen(output_filename, "w");
    if (!output)
    {
        perror("Erro ao criar arquivo de saída");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char encrypted[TAMANHO_SENHA + 1];
    while (fscanf(file, "%4s", encrypted) == 1)
    {
        char result[TAMANHO_SENHA + 1] = {0};
        brute_force(encrypted, result);
        fprintf(output, "%s\n", result);
    }

    fclose(file);
    fclose(output);

    printf("Processo PID %d: Senhas quebradas salvas em %s\n", getpid(), output_filename);
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <diretório de senhas> <diretório de resultados>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int num_files = 10;
    char filenames[10][256];
    for (int i = 0; i < num_files; i++)
    {
        snprintf(filenames[i], sizeof(filenames[i]), "%s/%03d.txt", argv[1], i + 1);
    }

    printf("Gerando %d processos para processar arquivos...\n", num_files);
    fflush(stdout);

    // Criando um processo separado para cada arquivo
    for (int i = 0; i < num_files; i++)
    {
        pid_t pid = fork();
        if (pid == 0) // Processo filho
        {
            process_file(filenames[i], argv[2]);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            perror("Erro ao criar processo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Aguardando término da força bruta]\n");
    fflush(stdout);

    // O processo pai aguarda todos os filhos terminarem
    while (wait(NULL) > 0)
        ;

    printf("[Fim dos processos descendentes]\n");
    fflush(stdout);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Tempo total de execução: %.2f ms\n", elapsed);
    fflush(stdout);

    return 0;
}
