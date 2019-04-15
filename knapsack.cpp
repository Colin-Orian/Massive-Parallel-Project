/*
Programer Name: Colin Orian
Program Descrpition: A brute force implementation of the 0/1 knapsack problem
see: https://en.wikipedia.org/wiki/Knapsack_problem#0/1_knapsack_problem
data set taken from https://www.geeksforgeeks.org/0-1-knapsack-problem-dp-10/ and
https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_multiple/knapsack_multiple.html
*/

#include <stdlib.h>
#include <vector>
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

//Parses through a .CSV file and return either vector or array of type Item
std::vector<Item> readVecCSV();
Item* readArrCSV(std::string fileName);

int max(int a, int b);
void knapsack(std::vector<struct Item> items, int carryWeight);
void knapsackArr(Item *item, int carryWeight);

int globalSize;

int main(int argc, char const *argv[]) {

    const rlim_t stackSize = sizeof(int) * 10000000000 * 10000000000;
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

    int carryWeight = 0;
    std::string fileName;

    if(argc > 1){
        carryWeight = std::stoi(argv[1]);
        fileName = argv[2];
    }else{
        carryWeight = 10000;
        fileName = "madeData.csv";
    }
    
  Item *itemsman = readArrCSV(fileName);
  auto start = std::chrono::system_clock::now();
  knapsackArr(itemsman, carryWeight);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> delta = end - start;
  std::cout << "Time to compute: " << delta.count() << std::endl;

  
  free(itemsman);
  return 0;
}
//knapsack function, but implimented in arrays
void knapsackArr(Item* items, int carryWeight){
  int knapsackMat[globalSize][carryWeight+1];
   //A backpack that has a carryWeight of 0 can't carry anything
  for(int y = 0; y < globalSize; y ++){
    knapsackMat[y][0] = 0;
  }
  
  for(int x = 1; x < carryWeight + 1; x ++){
    for(int y = 0; y < globalSize; y ++){
      struct Item currentItem = items[y];
      int actualVal;
      if(y == 0){
        if(currentItem.weight <= carryWeight){
          actualVal = currentItem.value;
        }else{
          actualVal = 0;
        }
      }else{
        if(currentItem.weight <= x){ //The current item can fit in the bag
          // What is better value? The all the previous items without the current item
          // or the current item plus the items that would be allowed with the current item inside the bag?
          actualVal = max(knapsackMat[y-1][x-currentItem.weight] + currentItem.value, knapsackMat[y-1][x]);
  
        }else{
          actualVal = knapsackMat[y-1][x];
        }
      }
      //std::printf("%d\n", actualVal);
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
//A function that reads through a csv file and returns a
//array
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
