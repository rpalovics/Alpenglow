import pandas as pd
import sys
from alpenglow.experiments import ExternalModelExperiment
from pathlib import Path
import os
import shutil

#generate the training files and test user lists using Alpenglow
base = Path("batches")
base.mkdir(parents=True, exist_ok=True)

data = pd.read_csv('http://info.ilab.sztaki.hu/~fbobee/alpenglow/tutorial_dataset.csv', header=None, names=['time', 'user', 'item'])

exp = ExternalModelExperiment(
    period_length=60 * 60 * 24 * 7 * 4,
    out_name_base="batches/batch",
    mode="write"
)
res = exp.run(data)

#reorganize files for RecBole
HEADER = "time:float,user:token,item:token,id:token,score:float,category:token\n"
N_BATCHES = 15  # 0..14 inclusive

for i in range(N_BATCHES):
    batch_dir = base / f"batch_{i}"
    trainfile = base / f"batch_{i}_train.dat"
    testfile = base / f"batch_{i}_test.dat"

    batch_dir.mkdir(parents=True, exist_ok=True)

    if trainfile.is_file():
        trainfile_new_name = f"batch_{i}.inter"
        trainfile_new = batch_dir / trainfile_new_name
        with open(trainfile, "r") as fin, open(trainfile_new, "w") as fout:
            fin.readline()        # discard old header
            fout.write(HEADER)
            for line in fin:
                fout.write(line)
        os.remove(trainfile)

    if testfile.is_file():
        testfile_new_name = f"batch_{i}_test.dat"
        testfile_new = batch_dir / testfile_new_name

        shutil.move(str(testfile), str(testfile_new))

