#! /bin/bash

cat raw101.out | ./inv_map.py > locs.txt
cat locs.txt > jennyIn101.out
cat forJennyIns.out >> jennyIn101.out

#rm locs.txt
#rm forJennyIns.out
#cat raw103.out | ./inv_map.py > locs.txt
#cat locs.txt > jennyIn103.out
#cat forJennyIns.out >> jennyIn103.out
#
#rm locs.txt
#rm forJennyIns.out
#cat raw104.out | ./inv_map.py > locs.txt
#cat locs.txt > jennyIn104.out
#cat forJennyIns.out >> jennyIn104.out

rm locs.txt
rm forJennyIns.out

cp jennyIn10* /home/walter/workspace/routing/.
