#!/bin/bash -eu

thisDir=$(dirname $0) || false

pushd $thisDir/../bin/test
echo $(pwd)
for i in $(find . -type f | grep -v \.o$); do
  echo $i;
  $i;
  echo;
done
popd
exit
