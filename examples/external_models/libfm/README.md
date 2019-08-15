# Libfm evaluation with Alpenglow

[Alpenglow](https://github.com/rpalovics/Alpenglow) makes it possible to evaluate recommender models in an online setting. This repository contains an example of connecting Alpenglow's evaluation with [libfm](https://github.com/srendle/libfm). However, since libfm itself does not provide a training objective for ranking or top_k prediction, we use the fork [libfm with BPR](https://github.com/fabiopetroni/libfm_with_BPR) which extends libfm with the Bayesian Personalized Ranking objective, and is capable of generating top K lists.

## How to run
```bash
git clone https://github.com/fabiopetroni/libfm_with_BPR.git
pushd libfm_with_BPR; make all; popd
python prepare_data.py
python prepare_libfm.py
./run_libfm.py
```

At this point, you can open the notebook `evaluate.ipynb` to evaluate the results.

## Implementation details

The simplest way of evaluating external batch models with Alpenglow is through the [ExternalModelExperiment](https://alpenglow.readthedocs.io/en/latest/alpenglow.experiments.html#alpenglow.experiments.ExternalModelExperiment.ExternalModelExperiment) interface of Alpenglow's API. This experiment has two modes of operation -- one is "write", which writes the training data to disk, and the other is "read", which reads the predictions made by the external model and evaluates them.

This experiment runs in 4 phases:
- The first one, `prepare_data.py` prepares the experiment by running `ExternalModelExperiment` in write mode. This writes train files and the list of corresponding test users who need toplists in the next time period.
- The second one, `prepare_libfm.py` transforms the data into the format required by [libfm with BPR](https://github.com/fabiopetroni/libfm_with_BPR). This format is documented in the linked repo.
- The third one, `run_libfm.sh` runs the libfm training and prediction process for all batches.
- Finally, the results are evaluated in `evaluate.ipynb`, once again using `ExternalModelExperiment`, this time in read mode.
