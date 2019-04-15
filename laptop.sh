g++ -o sequental.out knapsack.cpp
./seqScript.sh
mv itemsFixedSeq.dat ./laptop
mv capacityFixedSeq.dat ./laptop

g++ -o parallel.out knapsackP.cpp
./parallelScriptLaptop.sh
mv itemsFixedPar.dat ./laptop
mv capacityFixedPar.dat ./laptop
