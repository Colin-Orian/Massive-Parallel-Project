mkdir laptopOutput
g++ -o sequental.out knapsack.cpp
./seqScript.sh
mv itemsFixedSeq.dat ./laptopOutput
mv capacityFixedSeq.dat ./laptopOutput

g++ -o parallel.out knapsackP.cpp
./parallelScriptLaptop.sh
mv itemsFixedPar.dat ./laptopOutput
mv capacityFixedPar.dat ./laptopOutput
