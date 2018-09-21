Introduction
============

Welcome to Alpenglow introduction!

Alpenglow is an open source recommender systems research framework, aimed at providing tools for rapid prototyping and evaluation of algorithms for streaming recommendation tasks.

The framework is composed of a large number of components written in C++ and a thin python API for combining them into reusable experiments, thus enabling ease of use and fast execution at the same time. The framework also provides a number of preconfigured experiments in the :py:mod:`alpenglow.experiments` package and various tools for evaluation, hyperparameter search, etc.

Requirements
-------------

Anaconda environment with Python >= 3.5

Installing
----------

.. code-block:: bash

	conda install -c conda-forge alpenglow

Installing from source on Linux
-------------------------------

.. code-block:: bash

	cd Alpenglow
	conda install libgcc sip
	conda install -c conda-forge eigen
	pip install .

Development
-----------
.. role:: bash(code)
   :language: bash


- For faster recompilation, use :bash:`export CC="ccache cc"`
- To enable compilation on 4 threads for example, use :bash:`echo 4 > .parallel`
- Reinstall modified version using :bash:`pip install --upgrade --force-reinstall --no-deps .`
- To build and use in the current folder, :bash:`use pip install --upgrade --force-reinstall --no-deps -e .` and :bash:`export PYTHONPATH="$(pwd)/python:$PYTHONPATH"`
