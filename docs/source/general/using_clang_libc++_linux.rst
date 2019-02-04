Compiling Alpenglow using clang and libc++ on linux
===================================================

.. role:: bash(code)
   :language: bash

If you wish to compile Alpenglow using clang and libc++ instead of g++ and libstdc++ (for example to have the same behavior on linux and MacOS), you can do that with a few simple changes.

First, you need to set the CC environment variable to "clang++" by using the command :bash:`export CC="clang++"`. Next, you need to make the following changes to the "setup.py" file in the root directory of the package:

1. Under the platform specific flags for linux, add

.. code-block:: python

    '-stdlib=libc++',
    '-mfpmath=sse',
and remove

.. code-block:: python

    '-mfpmath=sse,387',

2. Add the following parameter to the 'alpenglow.cpp' extension in the call to the 'setup' function:

.. code-block:: python

    extra_link_args=[
        '-nodefaultlibs',
        '-nostdinc++',
        '-lc++',
        '-lm',
        '-lc',
        '-lgcc_s',
        '-lpthread',
        '-lgcc',
    ],

Now reinstall using :bash:`pip install --force-reinstall --no-deps --upgrade . `.
You can check if you were successful by running the following code:

.. code-block:: python

	import alpenglow
	print(alpenglow.cpp.__compiler, alpenglow.cpp.__stdlib)
	# expected output: clang libc++