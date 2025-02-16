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

