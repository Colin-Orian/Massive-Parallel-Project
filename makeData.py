import random
import time
import sys

numItems = int(sys.argv[1])
capacity = int(sys.argv[2])

MAX_VALUE = 100

outFile = open("madeData", 'w')
outFile.write(str(numItems) + "," + str(capacity) + "\n")
seed = time.time()
print("Seed is ", seed)
random.seed(seed)
for i in range(numItems):
    value = random.randint(1, MAX_VALUE)
    weight = random.randint(1, capacity)
    outFile.write(str(value)+","+str(weight)+"\n")
