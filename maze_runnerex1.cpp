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
    FILE* file = fopen(file_name, "r");
    
    if (file == nullptr) {
        // Trate o erro ao abrir o arquivo
        exit(1);
    }
    
    // Le o numero de linhas e colunas
    fscanf(file, "%d %d\n", &num_rows, &num_cols);
    
    // Aloca a matriz maze
    maze = new char*[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = new char[num_cols];
    }
    
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            // Le o valor da linha i, j do arquivo
            fscanf(file, " %c", &maze[i][j]);
            
            // Se o valor for 'e', salvar a posição inicial
            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
        fscanf(file, "\n");
    }
    
    fclose(file);
    
    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
    // Verifica se a posição atual é a saída
    if (maze[pos.i][pos.j] == 's') {
        // Limpa a tela
        system("clear");

        // Imprime o labirinto
        print_maze();
        
       std::this_thread::sleep_for(std::chrono::milliseconds(25));
        
        return true; // Saída encontrada
    }

    // Marca a posição atual como explorada
    maze[pos.i][pos.j] = '.';

    // Limpa a tela
    system("clear");
    // Imprime o labirinto
    print_maze();
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    // Verifica as próximas posições possíveis
    pos_t next_positions[] = {
        {pos.i - 1, pos.j}, // Up
        {pos.i + 1, pos.j}, // Down
        {pos.i, pos.j - 1}, // Left
        {pos.i, pos.j + 1}  // Right
    };

    for (pos_t next_pos : next_positions) {
        // Verifica se a próxima posição é válida
        if (next_pos.i >= 0 && next_pos.i < num_rows &&
            next_pos.j >= 0 && next_pos.j < num_cols &&
            (maze[next_pos.i][next_pos.j] == 'x' || maze[next_pos.i][next_pos.j] == 's')) {
            // Adiciona a próxima posição à pilha de posições válidas
            valid_positions.push(next_pos);
        }
    }

    // Verifica se a pilha de posições não está vazia
    if (!valid_positions.empty()) {
        pos_t next_position = valid_positions.top();
        valid_positions.pop();
        
        // Chama recursivamente a função walk com a próxima posição
        if (walk(next_position)) {
            return true; // Saída encontrada
        }
    }

    maze[pos.i][pos.j] = 'o';

    return false; // Não há saída a partir desta posição
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
