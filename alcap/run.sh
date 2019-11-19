#!/usr/bin/env bash

# Arguments
# Multitask macro name. Scripts searched for are $1_#.mac,
#   where # is the task number.
# Output filname prefix. Saved to output/$2_#.root
# Number of parallel tasks.

mkdir -p macros/parallel/
macro=$(basename $1)
for i in $(seq $3); do
	cp $1 macros/parallel/$macro.$i
	rnd1=$(od -vAn -N4 -td4 < /dev/urandom)
	rnd2=$(od -vAn -N4 -td4 < /dev/urandom)
	sed -i "s/setSeeds .*/setSeeds $rnd1 $rnd2/" macros/parallel/$macro.$i
done
seq $3 | parallel --eta --results output/jobs \
	"OFILENAMEROOT=output/$2.{}.root; g4sim macros/parallel/$macro.{}"
