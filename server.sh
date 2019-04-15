g++ -o sequental.out knapsack.cpp
./seqScript.sh
mv itemsFixedSeq.dat ./server
mv capacityFixedSeq.dat ./server

g++ -o parallel.out knapsackP.cpp
./parallelScriptServer.sh
mv itemsFixedPar.dat ./server
mv capacityFixedPar.dat ./server
