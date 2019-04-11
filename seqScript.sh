touch dataOutput.txt

for i in `seq 9`
do 
    echo "number of items 10^ $i" >> dataOutput.txt
    python3 makeData.py $((10**i)) >> dataOutput.txt
    for j in `seq 9`
    do
        echo "backpack size 10^ $i" >> dataOutput.txt
       ./a.out $((10**j)) "madeData.txt" >> dataOutput.txt
    done
done