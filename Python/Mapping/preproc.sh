#! /bin/bash

cat raw101_integrated | ./inv_map.py > locs.txt

cat locs.txt > jennyIn1.out
cat forJennyIns.out >> jennyIn1.out

rm locs.txt
rm forJennyIns.out

cp jennyIn1.out /home/walter/workspace/routing/
