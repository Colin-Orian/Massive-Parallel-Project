#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <sys/resource.h>
#include <chrono>
#include <ctime>

//A struct to represent a item in the knapsack
struct Item{
  int value;
  int weight;
};

struct thread_data{
    int thread_id;
    int **knapsackMat;
    int x;
    int y;
    struct Item currentItem;
};

Item* readArrCSV(std::string fileName);

int max(int a, int b);
void knapsackArrP(Item *item, int carryWeight, int num_thread);

void *kapsack(void *threadarg);

int num_thread;
int globalSize;
int carryWeight = 0;
bool stop_flag = false;

pthread_mutex_t mutextotal;


int main(int argc, char const *argv[]) {

    const rlim_t stackSize = sizeof(int) * 100000 * 100000;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if(result == 0){
        if(rl.rlim_cur < stackSize){
            rl.rlim_cur = stackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if(result !=0){
                std::fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    carryWeight = 0;
    std::string fileName;

    if(argc > 1){
        carryWeight = std::stoi(argv[1]);
        fileName = argv[2];
        num_thread = std::stoi(argv[3]);

    }else{
        carryWeight = 10;
        fileName = "madeData.csv";
        num_thread = 1;
    }
    
    Item *itemsman = readArrCSV(fileName);
    auto start = std::chrono::system_clock::now();
    knapsackArrP(itemsman, carryWeight, num_thread);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> delta = end - start;
    std::cout << "Time to compute: " << delta.count() << std::endl;

  
    free(itemsman);
    return 0;
}
//knapsack function, but implimented in arrays
void knapsackArrP(Item* items, int carryWeight, int num_thread){

  int knapsackMat[globalSize][carryWeight+1];
   //A backpack that has a carryWeight of 0 can't carry anything
  for(int y = 0; y < globalSize; y ++){
    knapsackMat[y][0] = 0;
  }

  pthread_t threads[num_thread];
  pthread_attr_t a;
  long i;
  int error;
  void *status;

  pthread_mutex_init(&mutextotal, NULL);
  pthread_attr_init(&a);
  pthread_attr_setdetachstate(&a, PTHREAD_CREATE_JOINABLE);

  int y =0;

  struct thread_data thread_data_array[num_thread];
    //for(int y = 0; y < globalSize; y ++){
        for(int x = 1; x < carryWeight + 1; x ++){
            for(int i = 1; i < num_thread; i++){
                if(y < globalSize){
                    thread_data_array[i].thread_id = x;
                    thread_data_array[i].x = x;
                    thread_data_array[i].y = y;
                    thread_data_array[i].currentItem = items[y];
                    thread_data_array[i].knapsackMat = (int **)&knapsackMat;

                    error = pthread_create(&threads[i],&a, kapsack, (void *)&thread_data_array[i]);
                    if (error) {
                        printf("ERROR: create() %d\n", error);
                        exit(-1);
                    }
                    y++;
                }
            }
        }
    //}
  std::cout << knapsackMat[globalSize-1][carryWeight] << std::endl;
}

int max(int a, int b){
  if(a > b){
    return a;
  }
  return b;
}

Item* readArrCSV(std::string fileName){
    std::string line;
    std::string value;
    std::string weight;
    std::ifstream file;

    //Opens the file
    file.open(fileName);

    //Check if the file exist or if there are any error
    if(!file){
        std::cerr << "Unable to open file!\n";
        exit(1);
    }

    //Since the first line of the file contains the size information
    //This will get the size info
    std::getline(file, line);

    //Since this function returns an pointer, which is imediatly going to get 
    //dealocated, this would mean //Item items[std::stoi(line)]; would just point
    //to an empty location. To counter this, we must allocate memory space and delete
    //afterward
    Item* items = (Item*) malloc(std::stoi(line) * sizeof(Item));

    //Unfortunatly, malloc returns a pointer which by default, returns a type void
    //We must track the size our self, hence global, as the global variable
    globalSize = std::stoi(line);
    int count = 0;

    //Read through the file line by line
    while(std::getline(file, line)){
        //Creates a string stream such that it fits the getline param
        std::stringstream ss(line);

        std::getline(ss,value, ',');
        std::getline(ss,weight, '\n');

        items[count].value = std::stoi(value);
        items[count].weight = std::stoi(weight);

        //std::printf("Value: %d Weight: %d i: %d\n",items[count].value, items[count].weight, count);
        count++;
    }
    return items;
}

void *kapsack(void *threadarg){
    struct thread_data *my_data;
    my_data = (struct thread_data*) threadarg;
    int actualVal;

    while(!stop_flag){
        if(my_data->x < carryWeight){
            if(my_data->y == 0){
                if(my_data->currentItem.weight <= carryWeight){
                    actualVal = my_data->currentItem.value;
                }else{
                    actualVal = 0;
                }
            }else{
                if(my_data->currentItem.weight <= my_data->x){
                    actualVal = max(my_data->knapsackMat[my_data->y-1][my_data->x - my_data->currentItem.weight]
                                + my_data->currentItem.value, my_data->knapsackMat[my_data->y-1][my_data->x]);
                }else{
                    actualVal = my_data->knapsackMat[my_data->y-1][my_data->x];
                }

            }

        my_data->knapsackMat[my_data->y][my_data->x] = actualVal;
        my_data->x += num_thread;
        }else{//Prop put mutex on this
            pthread_mutex_lock(&mutextotal);
            stop_flag = true;
            pthread_mutex_unlock(&mutextotal);
        }
    }
    pthread_exit(NULL);
}