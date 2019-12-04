Python API
=======================================

.. role:: python(code)
   :language: python

General information
###################

Alpenglow grew out of an internal framework that was originally written and used in C++. The basic premise of this framework was that a large number of componenst were implemented that one could piece together to create experiments. When we decided to provide a Python framework, this architecture stayed in place, only the framework now also provides precomposed, parametrizable experiments.

This document serves to describe the overall structure and workings of the Python package.

Python bindings in SIP
++++++++++++++++++++++

We use `SIP <https://www.riverbankcomputing.com/static/Docs/sip/>`_ to create Python bindings for C++ classes and interfaces. This allows us to access almost the entirety of the C++ codebase from Python. Python types like lists dictinaries and tuples are automatically converted to C++ stdlib types, and we can instantiate or even subclass C++ classes from Python. There are some caveats though, which we list under the appropriate sections here.

Type conversion
+++++++++++++++

Generally type conversion work automatically (if the type conversion is implemented in SIP files, see the *sip/std* folder). However generally conversions must do a copy of the data, thus incurring additional memory requirements. This is not a problem when model hyperparameters are copied, but with training datasets it can become problematic. If you are short on memory, what you can do is to run the experiments on data that is read from file, so it is directly read into the appropriate C++ structures and skips Python.

Instantiating C++ classes in Python
+++++++++++++++++++++++++++++++++++

C++ classes can be instantiated in Python without problem. However, since C++ doesn't support named arguments, these have a special way of being implemented. When a class instance is being created and named parameters are provided for a class named *Klass*, the system looks for a C++ struct named *KlassParameters* and sets the same named attribute on this, then provides it to the constructor of the C++ class *Klass* as its single parameter. This convention is followed throughout the codebase.

This process is done by the class :py:class:`alpenglow.Getter`: instead of importing the package *alpenglow.cpp*, you should include this class and use it described in the examples.

One more thing this class does is help with memory management. Since Python is garbage collected and C++ is not, memory management takes a little extra effort. Specifically, when a C++ class is instantiated in Python, is given to another C++ class instance, but then Python loses the reference, this causes a problem: the Python garbage collector frees the memory and the C++ code segfaults. This is avoided by keeping a reference of the objects in Python, tied to the experiment instance that is being run. This is also handled by the class :py:class:`alpenglow.Getter`: it can be asked to keep a reference of all objects created through it. For an example of this, see the implementation of the *run* method of :py:class:`alpenglow.OnlineExperiment`.

Implementing C++ interfaces in Python
+++++++++++++++++++++++++++++++++++++

Implementing C++ interfaces in Python generally works well. The main caveat is that multiple inheritance is not supported by SIP, thus it may be advisable to create a C++ interaface first specifically for being subclassed in Python. Another way to "cheat" this is to use composition instead of inheritance. You can see an example of this in the class :py:class:`alpenglow.PythonModel.SelfUpdatingModel`.

GIL and multithreading
++++++++++++++++++++++

While Python doesn't play nice with multithreading because of the global interpreter lock, this restriction does not apply to C++ code. Alpenglow explicitly releases the GIL at the start of the run of experiments, thus in theory multiple experiments can be run simultaneously in the same process. However, this is not very well tested, so use it carefully.

Package structure
#################

The package is divided into three main parts. The first one is the most often used one, the online recommendation experiment API. This spans over the root package and the  *experiments*, *eval*, and *utils* subpackages. The other two are *alpenglow.cpp* which provides raw access to the C++ classes and *alpenglow.offline* which stores the scikit-learn like api.