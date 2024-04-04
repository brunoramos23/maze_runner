#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <future>

// Matriz de char representnado o labirinto
char **maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;
bool finalizado;
// Representação de uma posição
struct pos_t
{
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
pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	FILE *file_maze = fopen(file_name, "r");
	// Abre o arquivo para leitura (fopen)

	fscanf(file_maze, "%d %d\n", &num_rows, &num_cols);
	// Le o numero de linhas e colunas (fscanf)
	// e salva em num_rows e num_cols

	maze = (char **)malloc(num_rows * sizeof(char *));
	// Aloca a matriz maze (malloc)
	for (int i = 0; i < num_rows; ++i)
		maze[i] = (char *)malloc(num_cols * sizeof(char));
	// Aloca cada linha da matriz
	char barran;

	for (int i = 0; i < num_rows; ++i)
	{

		for (int j = 0; j < num_cols; ++j)
		{
			fscanf(file_maze, "%c", &maze[i][j]);

			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
		}
		barran = fscanf(file_maze, "%c", &barran);
	}
	fclose(file_maze);
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t current_pos)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	int i = current_pos.i;
	int j = current_pos.j;

	if (i + 1 > num_rows && maze[current_pos.i + 1][current_pos.j] == 's')
	{
		finalizado = true;
	}

	if (i - 1 > -1 && maze[current_pos.i - 1][current_pos.j] == 's')
	{
		finalizado = true;
	}

	if (j + 1 < num_cols && maze[current_pos.i][current_pos.j + 1] == 's')
	{
		finalizado = true;
	}

	if (j - 1 > -1 && maze[current_pos.i][current_pos.j - 1] == 's')
	{
		finalizado = true;
	}

	if (j + 1 < num_cols && (maze[i][j + 1] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.j = j + 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (j - 1 > -1 && (maze[i][j - 1] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.j = j - 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (i + 1 < num_rows && (maze[i + 1][j] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.i = i + 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (i - 1 > -1 && (maze[i - 1][j] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.i = i - 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (finalizado == true)
	{
		return true;
	}
	return false;
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	// Marcar a posição atual com o símbolo '.'
	// Limpa a tela
	// Imprime o labirinto

	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		cada uma delas no vetor valid_positions
			- pos.i, pos.j+1
			- pos.i, pos.j-1
			- pos.i+1, pos.j
			- pos.i-1, pos.j
		Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	*/

	// Verifica se a pilha de posições nao esta vazia
	// Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
	// Caso contrario, retornar falso
}

int main(int argc, char *argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze2.txt");
	// chamar a função de navegação
	std::thread walk_thread(walk, initial_pos);

	while (!finalizado)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		system("clear || maze");
		print_maze();
	}
	// print_maze();
	if (finalizado) {
		std::cout << "Saída encontrada" << std::endl;
	}

	return 0;
}