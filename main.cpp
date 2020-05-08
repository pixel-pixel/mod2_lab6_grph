#include <iostream>
#include <fstream>

using namespace std;

//Однозв'язний граф
//Вершини графа - індекси(починаючи від нуля)
//При об'єднанні вершин графу(наприклад переміщуємо вершину 3 до вершини 5),
//то вершина 3 зникає, а індекси пересуваються:
//вершина 4 тепер стає вершиною 3, 5->4, 6->5 і т.д.
//Якщо граф має 3 вершини(0, 1, 2) і ви з'єднуєте наприклад вершину 1 із вершиною 5,
//то розмір графу автоматично збвльшується до 6 (адже індексація з нуля: 5+1 = 6)
//При видалені ребере розмір зменшуватись не буде.

struct Graph{
    int size;
    int matrix_size = 2;
    bool **matrix = new bool* [matrix_size];

    Graph(int size){
        for(int i = 0; i < matrix_size; i++){
            matrix[i] = new bool [matrix_size];
        }
        clear();

        this->size = size;
        if(size > matrix_size) change_matrix_size(size / 2 * 3);
    }

    void change_matrix_size(int new_size){
        bool **new_matrix = new bool* [new_size];

        for(int i = 0; i < new_size; i++){
            new_matrix[i] = new bool [new_size];

            for(int j = 0; j < new_size; j++){
                if(i < size && j < size) new_matrix[i][j] = matrix[i][j];
                else new_matrix[i][j] = false;
            }
        }
        swap(matrix, new_matrix);

        for(int i = 0; i < size; i++){
            delete [] new_matrix[i];
        }

        matrix_size = new_size;
    }

    void add_edge(int from, int to){
        int max;
        if(from > to) max = from;
        else max = to;

        if(max+1 > matrix_size){
            change_matrix_size((max+1)/2*3);
            size = max+1;
        }

        matrix[from][to] = true;
    }

    void del_edge(int from, int to){
        if(from < size && to < size){
            matrix[from][to] = false;
        }
    }

    void merge(int old, int in){
        for(int i = 0; i < size; i++){
            if(i != old){
                if(matrix[i][old]) matrix[i][in] = true;
                if(matrix[old][i]) matrix[in][i] = true;
            }
        }

        if(old == size-1){
            for(int i = 0; i < size; i++){
                matrix[i][old] = false;
            }
            for(int j = 0; j < size; j++){
                matrix[old][j] = false;
            }
        } else {
            for (int i = old; i < size - 1; i++) {
                for (int j = 0; j < size; j++) {
                    matrix[i][j] = matrix[i + 1][j];
                }
            }
            for (int i = 0; i < size; i++) {
                for (int j = old; j < size - 1; j++) {
                    matrix[i][j] = matrix[i][j + 1];
                }
            }
        }
        size--;
    }

    int* matrix_to_arr(){
        int arr_size = 0;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matrix[i][j]) arr_size++;
            }
        }

        ++arr_size *=2;

        int * arr = new int[arr_size];
        int index = 0;
        arr[index++] = arr_size;
        arr[index++] = size;

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matrix[i][j]) {
                    arr[index++] = i;
                    arr[index++] = j;
                }
            }
        }
        return arr;
    }

    void arr_to_matrix(int *arr){
        clear();
        int n = arr[0];
        for(int i = 2; i < n; i+=2){
            add_edge(arr[i], arr[i+1]);
        }
        delete [] arr;
    }

    void write_matrix(){
        ofstream out;
        out.open("/home/andrew/CLionProjects/mod2_lab6_grph/file", ios::binary);
        int *arr = matrix_to_arr();
        int n = arr[0];
        if(out.is_open()) {
            out.write((char*)&(*arr), sizeof(int)*n);
            out.close();
        }
        delete [] arr;
    }

    void read_matrix(){
        ifstream in;
        in.open("/home/andrew/CLionProjects/mod2_lab6_grph/file", ios::binary);
        if(in.is_open()){
            in.seekg(0, ios::end);
            int n = in.tellg() / 4;

            in.seekg(0, ios::beg);

            int *arr = new int[n];
            in.read((char*)&(*arr), sizeof(int) * n);
            arr_to_matrix(arr);
            in.close();
        }
    }

    void clear(){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                matrix[i][j] = false;
            }
        }
    }

    void print(){
        for(int j = 0; j < size; j++){
            for(int i = 0; i < size; i++){
                cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    Graph gr(2);
    gr.add_edge(0, 3);
    gr.add_edge(3, 2);
    gr.add_edge(2, 2);
    gr.write_matrix();
    gr.read_matrix();
    gr.print();
}
