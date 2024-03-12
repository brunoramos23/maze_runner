//abrir o arquivo, ler a matriz, procurar o e, procurar em volta o x


#include <stdio.h>
#include <stack>
#include <cstdlib>
#include <thread>
#include <unistd.h>

using namespace std;

// Matriz de char representando o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
    pos_t initial_pos;
    // Abre o arquivo para leitura (fopen)
	FILE* file = fopen(file_name, "r");

	// Verifica se o arquivo foi aberto com sucesso
	if (file == NULL) {
		printf("Erro ao abrir o arquivo '%s'\n", file_name);
		return initial_pos;
	}

	// Lê o número de linhas e colunas (fscanf) e salva em num_rows e num_cols
	fscanf(file, "%d %d", &num_rows, &num_cols);
    
    // Aloca a matriz maze (malloc)
	maze = (char**)malloc(sizeof(char*) * num_rows);
	for (int i = 0; i < num_rows; ++i) {
		maze[i] = (char*)malloc(sizeof(char) * num_cols);
	}

	// Lê o labirinto caractere a caractere (fgetc)
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			maze[i][j] = (char)fgetc(file);
            //fgetc lê o arquivo caractere a caractere
			// Se o caractere for 'e', salva a posição inicial
			if (maze[i][j] == 'e') {
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}

	// Fecha o arquivo (fclose)
	fclose(file);

	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
    system("clear");
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
   // Marcar a posição atual com o símbolo '.'
	maze[pos.i][pos.j] = '.';

	// Imprimir o labirinto
	print_maze();

	// Verificar se a posição atual é a saída
	if (maze[pos.i][pos.j] == 's') {
		return true;
	}

	// Verificar as posições adjacentes (cima, baixo, esquerda, direita)
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			// Ignorar a posição atual
			if (i == 0 && j == 0) {
				continue;
			}

			// Calcular a posição adjacente
			pos_t next_pos;
			next_pos.i = pos.i + i;
			next_pos.j = pos.j + j;

			// Verificar se a posição adjacente é válida
			if (next_pos.i >= 0 && next_pos.i < num_rows &&
				next_pos.j >= 0 && next_pos.j < num_cols) {

				// Verificar se a posição adjacente é um caminho válido ('x')
				if (maze[next_pos.i][next_pos.j] == 'x') {
					// Chamar a função `walk` recursivamente para a posição adjacente
					if (walk(next_pos)) {
						return true;
					}
				}
			}
		}
	}

	// Se nenhuma saída foi encontrada, retornar falso
	return false;
}



int main() {
    /*if (argc != 2) {
        printf("Uso: %s <maze.txt>\n", argv[0]);
        return 1;
    }*/

    // Carrega o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze("../data/maze4.txt");

    // Chama a função de navegação
    bool exit_found = walk(initial_pos);

    // Trata o retorno imprimindo a mensagem
    if (exit_found) {
        printf("Saída encontrada!\n");
    } else {
        printf("Nenhuma saída encontrada.\n");
    }

    // Libera a memória alocada para o labirinto
    for (int i = 0; i < num_rows; ++i) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}

