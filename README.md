<p align="center">
  <img src="./misc/alpenglow.png" alt="alpenglow"/>
</p>

[![Build Status](https://travis-ci.com/rpalovics/Alpenglow.svg?branch=master)](https://travis-ci.com/rpalovics/Alpenglow)
[![Docs Status](https://readthedocs.org/projects/alpenglow/badge/?version=latest)](https://alpenglow.readthedocs.io/en/latest/)

- Alpenglow is a free and open source C++ framework with easy-to-use Python API.
- Alpenglow is capable of training and evaluating industry standard recommendation algorithms including variants of popularity, nearest neighbor, and factorization models.
- Besides batch training and evaluation, Alpenglow supports *online training* of recommendation models capable of adapting to concept drift in non-stationary environments.

### Requirements
- Anaconda + Python 3.5 or 3.6 environment
### Conda package:
[![Conda Version](https://img.shields.io/conda/vn/conda-forge/alpenglow.svg)](https://anaconda.org/conda-forge/alpenglow) [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/alpenglow.svg)](https://anaconda.org/conda-forge/alpenglow) [![Linux](https://img.shields.io/circleci/project/github/conda-forge/alpenglow-feedstock/master.svg?label=Linux)](https://circleci.com/gh/conda-forge/alpenglow-feedstock)
[![OSX](https://img.shields.io/travis/conda-forge/alpenglow-feedstock/master.svg?label=macOS)](https://travis-ci.org/conda-forge/alpenglow-feedstock)
[![Windows](https://img.shields.io/appveyor/ci/conda-forge/alpenglow-feedstock/master.svg?label=Windows)](https://ci.appveyor.com/project/conda-forge/alpenglow-feedstock/branch/master)
### Installation from conda repositories
- `conda install -c conda-forge alpenglow`

### Installation from source (linux only at the moment)
- `cd Alpenglow`
- `conda install libgcc sip`
- `conda install -c conda-forge eigen`
- `pip install .`


### Getting Started

```python
from alpenglow.experiments import FactorExperiment
from alpenglow.evaluation import DcgScore
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

data = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset")

factor_model_experiment = FactorExperiment(
    top_k=100,
    seed=254938879,
    dimension=10,
    learning_rate=0.14,
    negative_rate=100
)

rankings = factor_model_experiment.run(data, verbose=True)
rankings['dcg'] = DcgScore(rankings)
day = 86400
averages = rankings['dcg'].groupby((rankings['time']-rankings['time'].min())//day).mean()
plt.plot(averages)
```
- Online documentation: https://alpenglow.readthedocs.io/en/latest/

### Development
- For faster recompilation, use `export CC="ccache cc"`
- E.g. to enable compilation on 4 threads, use `echo 4 > .parallel`
- Reinstall modified version using `pip install --upgrade --force-reinstall --no-deps .`
- To build and use in the current folder, use `pip install --upgrade --force-reinstall --no-deps -e .` and `export PYTHONPATH="$(pwd)/python:$PYTHONPATH"`
