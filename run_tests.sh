#!/bin/sh

if [ ! -f "mbi" ]
then
	echo "Missing 'mbi' binary file. Type 'scons'."
	exit 1
fi

for file in tests/*in.txt
do
    echo -n "$file: "
    result=$(/usr/bin/time -f "%Us" ./mbi < $file | md5sum)
    ideal=$(cat ${file%_*}_out.txt  | md5sum)
	if [ ! "$result" = "$ideal" ]
	then  
  		echo "Fail."
	fi
done
