#!/bin/sh

if [ $# -ne 1 ]
then
	echo "Usage:   $0 SIZE"
	echo "Example: $0 100"
	exit 65
fi

in_filename=$1_in.txt
out_filename=$1_out.txt

rm -f $in_filename

for i in $(seq $1)
do
   echo -n a >> $in_filename
done

echo "" >> $in_filename

for i in $(seq $1)
do
   echo -n c >> $in_filename
done

echo "" >> $in_filename

for i in $(seq $1)
do
   echo -n t >> $in_filename
done

cp $in_filename $out_filename
echo "" >> $out_filename
echo -$1 >> $out_filename
