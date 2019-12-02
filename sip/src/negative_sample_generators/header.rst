Negative sample generators
--------------------------

All the samples in an implicit dataset are positive samples.  To make gradient
methods work, we need to provide negative samples too.  This module contains
classes that implement different negative sample generation algorithms.  These
classes implement :py:class:`alpenglow.cpp.NegativeSampleGenerator`.  The most
frequently used implementation is
:py:class:`alpenglow.cpp.UniformNegativeSampleGenerator`.

