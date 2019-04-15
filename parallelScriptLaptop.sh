touch itemsFixedPar.dat
rm itemsFixedPar.dat
touch itemsFixedPar.dat
echo "number of items fixed:"
python3.6 makeData.py 1000
for i in `seq 9`
do
    echo "Capacity = 10^$i" >> itemsFixedPar.dat
    ./parallel.out $((10**i)) "madeData.csv" 8 >> itemsFixedPar.dat
done

touch capacityFixedPar.dat
rm capacityFixedPar.dat
touch capacityFixedPar.dat
echo "Bag Capacity fixed"
for i in `seq 9`
do
    echo "Items = 10^$i" >> capacityFixedPar.dat
    python3.6 makeData.py $((10**i))
    ./parallel.out 1000 "madeData.csv" 8 >> capacityFixedPar.dat
done