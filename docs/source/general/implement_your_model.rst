Implementing your own C++ model
===============================

Creating C++ model class and google tests
-----------------------------------------

The models must inherit from :py:class:`alpenglow.cpp.Model`.

File `cpp/src/main/models/MyNewModel.h`:

.. code-block:: cpp

    #ifndef MY_NEW_MODEL_H
    #define MY_NEW_MODEL_H
    
    #include "Model.h"
    #include <gtest/gtest_prod.h>
    
    class MyNewModel
      : public Model
    {
      public:
        double prediction(RecDat* rec_dat) override;
    };
    
    #endif /* MY_NEW_MODEL_H */

File `cpp/src/main/models/MyNewModel.cpp`:

.. code-block:: cpp

    #include "MyNewModel.h"
    
    double MyNewModel::prediction(RecDat* rec_dat){
      return 0; //TODO method stub
    }

TODO: google test installation
Unit test in file `cpp/src/test/models/TestMyNewModel.cpp`:

.. code-block:: cpp

    #include <gtest/gtest.h>
    #include "../../main/models/MyNewModel.h"
    
    namespace {
    
    class TestMyNewModel : public ::testing::Test {
      public:
        TestMyNewModel(){}
        virtual ~TestMyNewModel(){}
        void SetUp() override {
        }
        void TearDown() override {
        }
    };
    
    } //namespace
    
    TEST_F(TestMyNewModel, test){
      MyNewModel model;
    }
    
    
    int main (int argc, char **argv) {
      testing::InitGoogleTest(&argc, argv);
      return RUN_ALL_TESTS();
    }

To compile your model and link the test binary, step into directory `cpp/src` and run `scons`.
To make compilation faster, you can run it on multiple threads, e.g. `scons -j4` uses 4 threads.
Note that all `.o` files are linked to all test binaries, so all of them are regenerated if any `.h` or `.cpp` file changes, that makes the linking process a bit slow.

The test binaries are generated to `cpp/bin/test`.

Making the new model available in python
----------------------------------------

To make the model available in python, you will need the appropriate `sip/src/models/MyNewModel.sip` file.
For simple C++ headers, the sip file can be easily generated using a script:

.. code-block:: bash

    sip/scripts/header2sip cpp/src/main/models/MyNewModel.h overwrite

Note that the conversion script may fail for too complicated C++ files and also for ones that do not follow the formatting conventions of the project.
To mark your header as automatically convertible, add the comment line

.. code-block:: cpp

    //SIP_AUTOCONVERT

to the header file.
However, the conversion does not run automatically before compiling, you need to run it yourself.

Include your sip file in `sip/recsys.sip` to turn on python compilation:

.. code-block:: sip

    %Include src/models/MyNewModel.sip

Then reinstall alpenglow:

.. code-block:: bash

   pip install --upgrade --force-reinstall --no-deps .

Now the new model is available in python:

.. code-block:: python

    import alpenglow.Getter as rs
    my_new_model = rs.MyNewModel()
    rd = rs.RecDat()
    rd.time = 0
    rd.user = 10
    rd.item = 3
    my_new_model.prediction(rd)

Constructor parameters
----------------------

The constructor parameters are organized into a struct, that's name is the same as the name of the class, appended `Parameters`.  To add a parameter named `fading_factor`, extend the header file:

.. code-block:: cpp

    struct MyNewModelParameters {
      double fading_factor = 0.9;
    };
    
    class MyNewModel
      : public Model
    {
      public:
        MyNewModel(MyNewModelParameters* params){
          fading_factor_ = params->fading_factor;
        }
      double prediction(RecDat* rec_dat) override;
      private:
        double fading_factor_ = 0;
    };

Update the unit test:

.. code-block:: cpp

  TEST_F(TestMyNewModel, test){
    MyNewModelParameters model_params;
    model_params.fading_factor = 0.5;
    MyNewModel model(&model_params);
  }

Recompile using `scons` before running the unit test.
If all is fine on the cpp level, update the sip file and reinstall the pyton package:

.. code-block:: bash

    sip/scripts/header2sip cpp/src/main/models/MyNewModel.h overwrite
    pip install --upgrade --force-reinstall --no-deps .

Now the parameters is available in python:

.. code-block:: python

    import alpenglow.Getter as rs
    my_new_model = rs.MyNewModel(fading_factor=0.8)
    rd = rs.RecDat()
    rd.time = 0
    rd.user = 10
    rd.item = 3
    my_new_model.prediction(rd)

Updater for the model
---------------------

TODO
Logger also.

Document your model
-------------------

TODO

Pull common data
----------------

TODO
autocalled_initialize

Implement further functions of the Model interface
--------------------------------------------------

TODO
Serialization, self_test, add.

Make evaluation faster
----------------------

TODO
RSI.
