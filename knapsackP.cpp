/*
@author Colin Orian, Ibrahim Mushtaq
@Descrption A parallel impelemntation of the 0/1 knapsack problem
*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <sys/resource.h>
#include <chrono>
#include <ctime>

//A struct to represent a item in the knapsack
struct Item{
  int value;
  int weight;
};

Item* readArrCSV(std::string fileName);

int max(int a, int b);
void knapsackArrP(Item *item, int carryWeight, int num_thread);

//A function for the thread to run 
void *kapsack(void *threadarg);

int num_thread;
int globalSize;
int carryWeight = 0;
bool stop_flag = false;


int main(int argc, char const *argv[]) {


    //Increase the stack limit
    const rlim_t stackSize = sizeof(int) * 1000000000 * 1000000000;
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
        fileName = "knapsack.csv";
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

    for(int x = 1; x < carryWeight + 1; x++){

        //omp_set_num_threads(num_thread);

        #pragma omp parallel for num_threads(num_thread)
        for(int y = 0; y < globalSize; y++){
            struct Item currentItem = items[y];
            int actualVal;

            if(y == 0){
                if(currentItem.weight < carryWeight){
                    actualVal = currentItem.value;
                }else{
                    actualVal = 0;
                }
            }else{
                if(currentItem.weight <= x){
                    actualVal = max(knapsackMat[y-1][x -currentItem.weight] + currentItem.value, knapsackMat[y-1][x]);
                }else{
                    actualVal = knapsackMat[y-1][x];
                }
            }
            knapsackMat[y][x] = actualVal;

        }
    }
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
