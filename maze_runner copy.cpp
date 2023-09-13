#include <stdlib.h>
#include <stdio.h>
#include <stack>

#include <chrono>
#include <thread>

#include <iostream>

// Matriz de char representnado o labirinto
char** maze;

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

pos_t initial_pos;

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
// std::stack<pos_t> valid_positions;

//Flag saída encontrada
bool exit_found = 0;

// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	
	// Abre o arquivo para leitura (fopen)
	FILE * maze_file;
	maze_file = fopen(file_name, "rt");

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(maze_file, "%d", &num_rows);
	fscanf(maze_file, "%d", &num_cols);
	fgetc(maze_file);

	int i, j;

	// Aloca a matriz maze (malloc)
	maze = (char**) malloc (num_rows * sizeof (char*));

	for (int i=0; i < num_rows; i++){
		maze[i] = (char*) malloc (num_cols * sizeof (char));
	}

	// percorre a matriz
	for (i=0; i < num_rows; i++){
		for (j=0; j < num_cols; j++){
			maze[i][j] = 0 ;
		}
	}

	
	for (i = 0; i < num_rows; ++i) {
		for (j = 0; j < num_cols; ++j) {
			fscanf(maze_file, "%c", &maze[i][j]);
			if(maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
		fgetc(maze_file);
	}
	return initial_pos;
}

// Função que imprime o labirinto - Para tempos de sleep menores que 100ms
// void print_maze() {
// 	for (int i = 0; i < num_rows; ++i) {
// 		for (int j = 0; j < num_cols; ++j) {
// 			printf("%c", maze[i][j]);
// 		}
// 		printf("\n");
// 	}
// }

// Função que imprime o labirinto colorido
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			if(maze[i][j] == 'o' || maze[i][j] == '.'){
				printf("\33[1;31m");
				printf("%c", maze[i][j]);
			} else {
				printf("\33[1;30m");
				printf("%c", maze[i][j]);
			}
		}
		printf("\n");
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos) {

    std::stack<pos_t> valid_positions;
    pos_t valid_pos, next_position;

	maze[pos.i][pos.j] = 'o';
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	maze[pos.i][pos.j] = '.';

	if(pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 's'){
		maze[pos.i][pos.j+1] = 'o';
		system("clear");
		print_maze();
		exit_found = true;
	} else if(pos.j+1 < num_cols && maze[pos.i][pos.j+1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j+1;
		valid_positions.push(valid_pos);
	}

	if(0 <= pos.j-1 && maze[pos.i][pos.j-1] == 's'){
		maze[pos.i][pos.j-1] = 'o';
		system("clear");
		print_maze();
		exit_found = true;
	} else if(0 <= pos.j-1 && maze[pos.i][pos.j-1] == 'x') {
		valid_pos.i = pos.i;
		valid_pos.j = pos.j-1;
		valid_positions.push(valid_pos);
	}
	
	if(pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 's'){
		maze[pos.i+1][pos.j] = 'o';
		system("clear");
		print_maze();
		exit_found = true;
	} else if(pos.i+1 < num_rows && maze[pos.i+1][pos.j] == 'x') {
		valid_pos.i = pos.i+1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}
	
	if(0 <= pos.i-1 && maze[pos.i-1][pos.j] == 's'){
		maze[pos.i-1][pos.j] = 'o';
		system("clear");
		print_maze();
		exit_found = true;
	} else if(0 <= pos.i-1 && maze[pos.i-1][pos.j] == 'x') {
		valid_pos.i = pos.i-1;
		valid_pos.j = pos.j;
		valid_positions.push(valid_pos);
	}

   if (valid_positions.size() == 1) {
        next_position = valid_positions.top();
        valid_positions.pop();
        walk(next_position);
    } else if (valid_positions.size() == 2) {
        next_position = valid_positions.top();
        std::thread t0(walk, next_position);
        t0.detach();
        valid_positions.pop();
        next_position = valid_positions.top();
        walk(next_position);
    } else if (valid_positions.size() == 3) {
        next_position = valid_positions.top();
        std::thread t1(walk, next_position);
        t1.detach();
        valid_positions.pop();
        next_position = valid_positions.top();
        std::thread t2(walk, next_position);
        t2.detach();
        next_position = valid_positions.top();
        walk(next_position);
    }
}

int main(int argc, char* argv[]) {

	pos_t initial_pos = load_maze(argv[1]);
	
	std::thread t(walk, initial_pos);
	t.detach();

	while(!exit_found){
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		system("clear");
	}

	print_maze();
    printf("\33[0m");
	printf("Saida encontrada!\n");

	return 0;
}