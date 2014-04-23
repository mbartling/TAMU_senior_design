#! /bin/bash

# cp /home/walter/Code/rawData/raw101-14-04-20-* .
#cp /home/walter/Code/rawData/raw104-14-04-20-* .

cat raw101-14-04-20horiz.out | ./inv_map.py > locs.txt
cat locs.txt > jennyIn101horiz.out
cat forJennyIns.out >> jennyIn101horiz.out # append rssi matrix

cat raw101-14-04-20vert.out | ./inv_map.py > locs.txt
cat locs.txt > jennyIn101vert.out
cat forJennyIns.out >> jennyIn101vert.out

cat raw104-14-04-20horiz.out | ./inv_map.py > locs.txt
cat locs.txt > jennyIn104horiz.out
cat forJennyIns.out >> jennyIn104horiz.out # append rssi matrix

cat raw104-14-04-20vert.out | ./inv_map.py > locs.txt
cat locs.txt > jennyIn104vert.out
cat forJennyIns.out >> jennyIn104vert.out

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

cp jennyIn10* /home/walter/workspace/routing_polarized/.
cp xSpace.out /home/walter/workspace/routing_polarized/.
cp ySpace.out /home/walter/workspace/routing_polarized/.
