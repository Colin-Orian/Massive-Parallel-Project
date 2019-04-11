import random
import time
import sys

numItems = int(sys.argv[1])


MAX_VALUE = 100

outFile = open("madeData.txt", 'w')
outFile.write(str(numItems)+"\n")
seed = time.time()
print("Seed is ", seed)
random.seed(seed)
for i in range(numItems):
    value = random.randint(1, MAX_VALUE)
    weight = random.randint(1, 10)
    outFile.write(str(value)+","+str(weight)+"\n")
