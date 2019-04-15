touch itemsFixedSeq.dat
rm itemsFixedSeq.dat
touch itemsFixedSeq.dat
echo "number of items fixed:"
python3.6 makeData.py 1000
for i in `seq 9`
do
    echo "Capacity = 10^$i" >> itemsFixedSeq.dat
    ./sequental.out $((10**i)) "madeData.csv" >> itemsFixedSeq.dat
done

touch capacityFixedSeq.dat
rm capacityFixedSeq.dat
touch capacityFixedSeq.dat
echo "Bag Capacity fixed"
for i in `seq 9`
do
    echo "Items = 10^$i" >> capacityFixedSeq.dat
    python3.6 makeData.py $((10**i))
    ./sequental.out 1000 "madeData.csv" >> capacityFixedSeq.dat
done