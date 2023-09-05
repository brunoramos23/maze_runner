#include <iostream>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

// Matriz de char representando o labirinto
char** maze;

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Mutex para sincronização
mutex mtx;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

// Estrutura de dados contendo as próximas
// posições a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

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
    
    // Aloca a matriz original_maze
    char** original_maze = new char*[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        original_maze[i] = new char[num_cols];
    }
    
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            // Le o valor da linha i, j do arquivo
            fscanf(file, " %c", &maze[i][j]);
            
            // Copie o valor para a matriz original_maze
            original_maze[i][j] = maze[i][j];
            
            // Se o valor for 'e', salvar a posição inicial
            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
        fscanf(file, "\n");
    }
    
    fclose(file);

    // Chame a função explore com a matriz original_maze
    bool exit_found = explore(initial_pos, original_maze);

    // Trate o retorno imprimindo a mensagem
    if (exit_found) {
        cout << "Saída encontrada!" << endl;
    } else {
        cout << "Nenhuma saída encontrada." << endl;
    }

    // Libere a memória alocada para o labirinto e a matriz original_maze
    for (int i = 0; i < num_rows; ++i) {
        delete[] maze[i];
        delete[] original_maze[i];
    }
    delete[] maze;
    delete[] original_maze;

    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
    lock_guard<mutex> lock(mtx);
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

bool walk(pos_t pos, char** original_maze); // Declaração antecipada da função explore

void explore(pos_t pos, char** original_maze) {
    // Implemente a lógica de exploração a partir da posição "pos"

    // Exemplo de verificação da saída
    if (maze[pos.i][pos.j] == 's') {
        lock_guard<mutex> lock(mtx);
        print_maze();
        return;
    }

    // Implemente o movimento e a exploração das próximas posições
    maze[pos.i][pos.j] = '.';

    lock_guard<mutex> lock(mtx);
    print_maze();

    pos_t next_positions[] = {
        {pos.i - 1, pos.j}, // Up
        {pos.i + 1, pos.j}, // Down
        {pos.i, pos.j - 1}, // Left
        {pos.i, pos.j + 1}  // Right
    };

    vector<thread> threads;

    for (pos_t next_pos : next_positions) {
        if (next_pos.i >= 0 && next_pos.i < num_rows &&
            next_pos.j >= 0 && next_pos.j < num_cols &&
            (maze[next_pos.i][next_pos.j] == 'x' || maze[next_pos.i][next_pos.j] == 's')) {
            threads.emplace_back(walk, next_pos, original_maze);
        }
    }

    // Aguarde até que todas as threads de exploração terminem
    for (thread& t : threads) {
        t.join();
    }

    maze[pos.i][pos.j] = 'o';
}

int main(int argc, char* argv[]) {
   // if (argc != 2) {
     //   cout << "Uso: " << argv[0] << " <maze.txt>" << endl;
      //  return 1;
    //}

    // Carrega o labirinto com o nome do arquivo recebido como argumento
    //pos_t initial_pos = load_maze(argv[1]);

    //return 0;
}
