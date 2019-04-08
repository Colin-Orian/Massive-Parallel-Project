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

//A struct to represent a item in the knapsack
struct Item{
  int value;
  int weight;
};

//Parses through a .CSV file and return either vector or array of type Item
std::vector<Item> readVecCSV();
Item* readArrCSV();

//Finds the sum of all the values of the item
int sum(std::vector<struct Item> items);
int max(int a, int b);
/*
Prints out the optimal solution of the knapsack problem with the given weights
*/
void knapsack(std::vector<struct Item> items, int carryWeight);
void knapsackArr(Item *item, int carryWeight);

int globalSize;

int main(int argc, char const *argv[]) {

  //Dummy data
  std::vector<struct Item> items;
  items.push_back(Item());
  items[0].value = 50;
  items[0].weight = 10;

  items.push_back(Item());
  items[1].value = 100;
  items[1].weight = 20;

  items.push_back(Item());
  items[2].value = 120;
  items[2].weight = 30;

  int carryWeight = 5;

    std::vector<struct Item> dummy = readVecCSV();
    Item *itemsman = readArrCSV();

  knapsack(dummy, carryWeight);
  knapsackArr(itemsman, carryWeight);
  free(itemsman);
  return 0;
}

//https://www.youtube.com/watch?v=8LusJS5-AGo
void knapsack(std::vector<struct Item> items, int carryWeight){
  int knapsackMat[items.size()][carryWeight+1];
   //A backpack that has a carryWeight of 0 can't carry anything
  for(int y = 0; y < items.size(); y ++){
    knapsackMat[y][0] = 0;
  }
  
  for(int x = 1; x < carryWeight + 1; x ++){
    for(int y = 0; y < items.size(); y ++){
      struct Item currentItem = items[y];
      int actualVal;
      if(y == 0){
        if(currentItem.weight <= carryWeight){
          actualVal = currentItem.value;
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
  std::cout << knapsackMat[items.size()-1][carryWeight] << std::endl;
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

int sum(std::vector<struct Item> items){
  int sum = 0;
  for(int i = 0; i < items.size(); i ++){
    sum += items[i].value;
  }
  return sum;
}

int max(int a, int b){
  if(a > b){
    return a;
  }
  return b;
}

//A function that reads through a csv file
//The structure of that CSV file is value,weight
std::vector<Item> readVecCSV(){
    std::vector<struct Item> items;
    std::string line;
    std::string value;
    std::string weight;
    std::ifstream file;

    //Tries to open the file
    file.open("knapsack.csv");

    //Check if the file cannot be opened
    if(!file){
        std::cerr << "Unable to open file!\n";
        exit(1);
    }

    std::getline(file, line);

    //Read through the file line by line
    while(std::getline(file, line)){
        //Creates a string stream such that it fits the getline param
        std::stringstream ss(line);

        std::getline(ss,value, ',');
        std::getline(ss,weight, '\n');

        items.push_back(Item());
        items[items.size() - 1].value = std::stoi(value);
        items[items.size() - 1].weight = std::stoi(weight);
    }

    return items;
}

//A function that reads through a csv file and returns a
//array
Item* readArrCSV(){
    std::string line;
    std::string value;
    std::string weight;
    std::ifstream file;

    //Opens the file
    file.open("knapsack.csv");

    //Check if the file exist or if there are any error
    if(!file){
        std::cerr << "Unable to open file!\n";
        exit(1);
    }

    //Since the first line of the file contains the size information
    //This will get the size info
    std::getline(file, line);
    std::cout << line << std::endl;

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