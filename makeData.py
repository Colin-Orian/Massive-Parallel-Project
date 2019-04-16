"""
@author Colin Orian
@Description Used to generate a dataset for the algorithm to run on
"""
import random
import time
import sys

numItems = int(sys.argv[1])


MAX_VALUE = 100

outFile = open("madeData.csv", 'w')
outFile.write(str(numItems)+"\n") #number of items is the header
seed = time.time()
random.seed(seed)
for i in range(numItems):
    value = random.randint(1, MAX_VALUE)
    weight = random.randint(1, 10)
    outFile.write(str(value)+","+str(weight)+"\n")
