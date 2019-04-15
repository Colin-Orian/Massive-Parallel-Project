mkdir serverOutput
g++ -std=c++11 -o sequental.out knapsack.cpp
./seqScript.sh
mv itemsFixedSeq.dat ./serverOutput
mv capacityFixedSeq.dat ./serverOutput

g++ -std=c++11 -o parallel.out knapsackP.cpp
./parallelScriptServer.sh
mv itemsFixedPar.dat ./serverOutput
mv capacityFixedPar.dat ./serverOutput
