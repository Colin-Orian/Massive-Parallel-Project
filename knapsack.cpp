/*
Programer Name: Colin Orian
Program Descrpition: A brute force implementation of the 0/1 knapsack problem
see: https://en.wikipedia.org/wiki/Knapsack_problem#0/1_knapsack_problem
data set taken from https://www.geeksforgeeks.org/0-1-knapsack-problem-dp-10/
*/

#include <stdlib.h>
#include <vector>
#include <iostream>

//A struct to represent a item in the knapsack
struct Item{
  int value;
  int weight;
};

//Finds the sum of all the values of the item
int sum(std::vector<struct Item> items);
int max(int a, int b);
/*
Prints out the optimal solution of the knapsack problem with the given weights
*/
void knapsack(std::vector<struct Item> items, int carryWeight);
/*
Recursively computes the knapsack problem on subsets of the actual set
*/
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

  int carryWeight = 50;
  knapsack(items, carryWeight);
  
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
      knapsackMat[y][x] = actualVal;
    }
  }
  std::cout << knapsackMat[items.size()-1][carryWeight] << std::endl;
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
