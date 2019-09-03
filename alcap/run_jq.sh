#!/usr/bin/env bash

# Arguments
# Multitask macro name.
# Output filname prefix. Saved to output/$2_#.root
# Number of parallel tasks.

mkdir -p macros/parallel/
macro=$(basename $1)
for i in $(seq $3); do
	cp $1 macros/parallel/$macro.$i
	rnd=$(od -vAn -N4 -td4 < /dev/urandom)
	sed -i "s/setSeeds .*/setSeeds $rnd/" macros/parallel/$macro.$i
done

mkdir -p work
seq $3 | parallel --eta --results output/jobs \
	"cd \$(mktemp -d work/{}.XXXXX); OFILENAMEROOT=$ALCAPWORKROOT/output/$2.{}.root; g4sim $ALCAPWORKROOT/macros/parallel/$macro.{}"
