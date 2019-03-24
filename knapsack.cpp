/*
Programer Name: Colin Orian
Program Descrpition: A brute force implementation of the 0/1 knapsack problem
see: https://en.wikipedia.org/wiki/Knapsack_problem#0/1_knapsack_problem
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
/*
Prints out the optimal solution of the knapsack problem with the given weights
*/
void knapsack(std::vector<struct Item> items, int carryWeight);
/*
Recursively computes the knapsack problem on subsets of the actual set
*/
std::vector<struct Item> _knapsack(std::vector<struct Item> items, int carryWeight);

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


void knapsack(std::vector<struct Item> items, int carryWeight){
  std::vector<struct Item> solution = _knapsack(items, carryWeight);

  std::cout << sum(solution) << std::endl;
}

std::vector<struct Item> _knapsack(std::vector<struct Item> items, int carryWeight){
  //It's the empty set so return the empty set
  if(items.size() == 0){
    std::vector<struct Item> nothing;
    return nothing;
  }
  //Create the subset
  std::vector<struct Item> subset;
  for(int i = 0; i < items.size()-1; i ++){
    subset.push_back(items[i]);
  }
  //Get the items the solution of the knapsack problem excluding the right most item
  std::vector<struct Item> leftItems = _knapsack(subset, carryWeight);
  //The right most item is too heavy to put in. Therefore, best solution doesn't include it
  if(items[items.size()-1].weight > carryWeight){
    return leftItems;
  }
  //Computes the solution including the current item
  std::vector<struct Item> rightItems = _knapsack(subset, carryWeight -items[items.size()-1].weight);

  //Return whichever solution has the highest value
  int leftValue = sum(leftItems);
  int rightValue = sum(rightItems);
  if(leftValue >= rightValue + items[items.size()-1].value){
    return leftItems;
  }else{
    rightItems.push_back(items[items.size()-1]);
    return rightItems;
  }
}

int sum(std::vector<struct Item> items){
  int sum = 0;
  for(int i = 0; i < items.size(); i ++){
    sum += items[i].value;
  }
  return sum;
}
