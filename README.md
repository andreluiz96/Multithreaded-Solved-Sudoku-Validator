# MULTITHREADED SOLVED SUDOKU VALIDATOR
Este programa verifica se uma solução de Sudoku é válida ou não. Ele recebe um arquivo de entrada contendo o tabuleiro de Sudoku e as dimensões das subgrades, e retorna o resultado em um arquivo chamado "sudoku_albgm.out".

Requisitos
Compilador GCC
Bibliotecas pthread, stdio, stdlib, string e math

Como usar
Compile o programa com o comando:
gcc nome_do_arquivo.c -o nome_do_executavel -lpthread

Execute o programa passando o arquivo de entrada como argumento:
./nome_do_executavel arquivo_de_entrada.txt

O resultado da verificação será salvo no arquivo "sudoku_albgm.out". Se o Sudoku estiver resolvido corretamente, o arquivo conterá "SUCCESS". Se houver algum erro na resolução, o arquivo conterá "FAIL".

Formato do arquivo de entrada
O arquivo de entrada deve conter primeiro as dimensões do tabuleiro e das subgrades (ex: "9x9\n3x3\n"), seguido pelos números do tabuleiro separados por espaços e quebras de linha.

Exemplo de arquivo de entrada:<br>
9x9<br>
3x3<br>
6 2 4 5 3 9 1 8 7<br>
5 1 9 7 2 8 6 3 4<br>
8 3 7 6 1 4 2 9 5<br>
1 4 3 8 6 5 7 2 9<br>
9 5 8 2 4 7 3 6 1<br>
7 6 2 3 9 1 4 5 8<br>
3 7 1 9 5 6 8 4 2<br>
4 9 6 1 8 2 5 7 3<br>
2 8 5 4 7 3 9 1 6<br>


## Explicação do código

O programa utiliza várias funções e uma estrutura para verificar a solução do Sudoku:

- `validar_linhas`: Esta função é responsável por validar se os números nas linhas não se repetem.
- `validar_colunas`: Esta função é responsável por validar se os números nas colunas não se repetem.
- `checar_submatriz`: Esta função verifica se os números em uma submatriz específica não se repetem.
- `validar_submatriz`: Esta função é responsável por validar se os números nas submatrizes não se repetem.
- `file_len`: Esta função retorna a quantidade de linhas em um arquivo.
- `check_last_char`: Esta função verifica o último caractere do arquivo para garantir que oarquivo esteja no formato correto.

A estrutura thread_arg_t é usada para passar os argumentos para as funções de validação executadas pelas threads. Esses argumentos incluem as posições iniciais e finais das linhas e colunas e as dimensões das submatrizes.

No main, o programa começa abrindo e lendo o arquivo de entrada e verifica o formato correto. Em seguida, ele lê as dimensões da matriz e das submatrizes e armazena na memória a matriz dinâmica matrizD.

Após isso, são criadas várias threads para validar as linhas, colunas e submatrizes do Sudoku, dividindo a carga de trabalho entre elas. As threads são criadas e executadas de acordo com as dimensões das submatrizes, e a estrutura thread_arg_t é usada para passar os argumentos necessários para as funções de validação.

Quando todas as threads terminam sua execução, o programa verifica se todas as validações foram bem-sucedidas (indicado pelas variáveis validador_linha, validador_coluna e validador_sub). Se todas as validações passarem, o programa escreve "SUCCESS" no arquivo de saída sudoku_albgm.out. Caso contrário, ele escreve "FAIL".

Por fim, o programa libera a memória alocada e fecha os arquivos. 
##  Exemplo de uso
Para usar o programa, compile-o com um compilador C e execute-o com um arquivo de entrada que contenha um Sudoku no formato especificado. Por exemplo:
gcc sudoku_validator.c -o sudoku_validator -lpthread
./sudoku_validator input.txt

Onde input.txt é um arquivo que contém o Sudoku que você deseja validar. O resultado da validação será escrito no arquivo sudoku_albgm.out.

Lembre-se de que o arquivo de entrada deve estar no formato correto, com as dimensões da matriz e das submatrizes na primeira e segunda linhas, respectivamente, seguidas pelos valores da matriz Sudoku.

## Limitações
Este programa foi projetado especificamente para validar Sudokus de tamanhos maiores que 9x9. Ele não verificará corretamente Sudokus menores que 9x9, e pode não funcionar corretamente com Sudokus que não sigam o formato de arquivo especificado. Além disso, o programa utiliza a biblioteca POSIX Threads (pthreads) para criar e gerenciar threads, o que pode não ser compatível com todos os sistemas operacionais e ambientes de desenvolvimento.
