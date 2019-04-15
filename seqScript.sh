touch itemsFixed.dat
echo "number of items fixed:"
python3 makeData.py 1000
for i in `seq 9`
do
    echo "Capacity = 10^$i" >> itemsFixed.dat
    ./a.out $((10**i)) "madeData.csv" >> itemsFixed.dat
done

touch capcityFixed.dat
echo "Bag Capacity fixed"
for i in `seq 9`
do
    echo "Items = 10^$i" >> capcityFixed.dat
    python3 makeData.py $((10**i))
    ./a.out 1000 "madeData.csv" >> capcityFixed.dat
done