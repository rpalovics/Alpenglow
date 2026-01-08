#!/bin/bash

for i in $(seq 0 14); do
  dir=batches/batch_$i
  trainfile_new=$dir/batch_$i.inter
  testfile_new=$dir/batch_${i}_test.dat
  testfile_orig=$dir/batch_${i}_test_orig.dat
  if [ -f $testfile_new ]; then
    mv $testfile_new $testfile_orig
    echo user > $testfile_new
    #filter users that doesn't exist in the training data, RecBole doesn't like these
    grep -Fxf $testfile_orig <(cut -d"," -f"2" $trainfile_new | sort -u ) >> $testfile_new
  fi
done

