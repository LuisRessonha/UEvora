#!/bin/bash
SIZE=$1
INPUTFILE=$2

cd Ficheiros\ Prof/
./fonte-linux64 $SIZE > $INPUTFILE
mv $INPUTFILE ..
cd ..
python3 countOccur.py $INPUTFILE
rm $INPUTFILE

