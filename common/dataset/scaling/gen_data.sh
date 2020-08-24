#!/bin/bash

EXT=.csv
DIR=common/dataset

rm -f *.csv

# generating strong scaling dataset
./$DIR/generator 11000 $DIR/scaling/STRONG$EXT

# generating weak scaling datasets
for (( i=10000; i<=20000; i+=1000 )); do
	n=$(( i / 4 ))
	./$DIR/generator $n $DIR/scaling/WEAK_$i$EXT
done
