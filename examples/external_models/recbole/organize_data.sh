#!/bin/bash

for i in $(seq 0 14); do
  dir=batches/batch_$i
  trainfile=batches/batch_${i}_train.dat
  trainfile_new=batch_$i.inter
  testfile=batches/batch_${i}_test.dat
  mkdir $dir
  if [ -f $trainfile ]; then
    mv $trainfile $dir/$trainfile_new
    sed -i '1s/.*/time:float,user:token,item:token,id:token,score:float,category:token/' $dir/$trainfile_new
  fi
  if [ -f $testfile ]; then
    mv $testfile $dir/batch_${i}_test_orig.dat
    echo user > $dir/batch_${i}_test.dat
    #filter users that doesn't exist in the training data, RecBole doesn't like these
    grep -Fxf $dir/batch_${i}_test_orig.dat <(cut -d"," -f"2" $dir/$trainfile_new | sort -u ) >> $dir/batch_${i}_test.dat
  fi
done

