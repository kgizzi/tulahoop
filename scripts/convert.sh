#!/bin/bash
for f in $1*.bmp ;
do
  echo "File: $f"
  python3 bin2header.py $f
  header=${f%.*p}
  echo "Header: $header.h"
  mv $header.h ../src/images/
  echo " "
done

echo "Includes..."
echo " \n\n"

for f in $1/*.bmp ;
do
  header=${f%.*p}
  header_rel=${header##/*/}
  echo "#include \"images/$header_rel.h\""
done

echo "\n\n\n\nImages List..."
echo " \n\n"

for f in $1*.bmp ;
do
  header=${f%.*p}
  header_rel=${header##/*/}
  echo "$header_rel,"
done
