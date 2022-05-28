#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

struct Vertex{
    int x;
    int y;
    static int size;
    Vertex* prev = nullptr;
    Vertex* next = nullptr;
    Vertex(int x, int y) : x(x), y(y) {
        size++;
    }
    bool operator==(const Vertex& v){
        return x == v.x && y == v.y;
    }
};

int Vertex::size = 0;

void upload(Vertex*& head, std::string filename);
void save(Vertex*& head, std::string filename);
double distance(const Vertex& v1, const Vertex& v2);
static void annealing(Vertex* head, int max_it);
double total_length(Vertex* head);


int main(void){
        
    srand(time(NULL));    
    Vertex* head = nullptr;
    upload(head, "data.txt");   
    // for(int _ = 0 ; _ < 5 ; _++){
        annealing(head, 100000);
        save(head, "result.dat");
        std::cout << total_length(head) << std::endl;
        for(int i = 0 ; i < Vertex::size-1 ; i++){
            delete head->prev;
            head = head->next;
        }
        Vertex::size = 0;
        // upload(head, "result.dat");   
    // }

    return 0;
}

void upload(Vertex*& head, std::string filename){
    std::ifstream input_file(filename);
    int x, y;
    if(input_file >> x >> y){
        head = new Vertex(x, y);
    }
    Vertex* tmp = head;
    while(input_file >> x >> y){
        tmp->next = new Vertex(x, y);
        tmp->next->prev = tmp;
        tmp = tmp->next;
    }
    tmp->next = head;
    head->prev = tmp;
}

void save(Vertex*& head, std::string filename){
    std::ofstream output_file;
    output_file.open (filename);
    Vertex* tmp = head;
    for(int i = 0 ; i < Vertex::size ; i++){
        output_file << tmp->x << " " << tmp->y << std::endl;
        tmp = tmp->next;
    }
    //Comment line below to read from the result file properly
    output_file << head->x << " " << head->y << std::endl;
    output_file.close();
}

double distance(const Vertex& v1, const Vertex& v2){
    return std::sqrt(std::pow(v1.x-v2.x, 2) + std::pow(v1.y-v2.y, 2));
}

double total_length(Vertex* head){
    double result = 0.0;
    Vertex* tmp = head;
    for(int i = 0 ; i < Vertex::size ; i++){
        result += distance(*tmp, *tmp->next);
        tmp = tmp->next;
    }
    return result;
}

static void annealing(Vertex* head, int max_it){
    for (int i = 100; i >= 1; i--){
        double T = 0.001*i*i;
        for (int it = 0; it < max_it; it++){    
            int i1 = rand() % Vertex::size;
            int i2 = 2 + rand() % (Vertex::size - 3);
            Vertex* v1 = head;
            Vertex* v2;
            for (i1; i1 > 0; i1--){
                v1 = v1->next;
            }
            v2 = v1;
            for(i2; i2 > 0; i2--){
                v2 = v2->next;
            }

            double new_len = distance(*v1, *v2) + distance(*v1->next, *v2->next);
            double current_len = distance(*v1, *v1->next) + distance(*v2, *v2->next);

            if(new_len < current_len || std::rand()/RAND_MAX < exp(-(new_len-current_len)/T)){
                Vertex* v2_next = v2->next;
                v2->next = v2->prev;
                v2->prev = v1;
                v1->next = v2;
                Vertex* tmp = v2->next;
                while(tmp->prev != v1){
                    Vertex* c = tmp->prev;
                    tmp->prev = tmp->next;
                    tmp->next = c; 
                    tmp = tmp->next;
                }
                tmp->prev = tmp->next;
                tmp->next = v2_next;
                v2_next->prev = tmp;
            }
        }
    }
}
