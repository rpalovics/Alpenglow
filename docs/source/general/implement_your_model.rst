Implementing your own C++ model
===============================

Creating C++ model class and google tests
-----------------------------------------

The models must inherit from :py:class:`alpenglow.cpp.Model`.  The bare minimal model implementation consists of the following two files.

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

You may need to install gtest in directory `cpp/dep` before compiling your code:

.. code-block:: bash

    cd cpp/dep
    mv gtest gtest_old
    wget https://github.com/google/googletest/archive/release-1.7.0.zip
    unzip -q release-1.7.0.zip
    mv googletest-release-1.7.0 gtest
    cd gtest
    GTEST_DIR=`pwd`
    mkdir build
    cd build
    g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread \
        -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o

To compile your model and link the test binary, step into directory `cpp/src` and run `scons`.
To make compilation faster, you can run it on multiple threads, e.g. `scons -j4` uses 4 threads.
Note that for the sake of simplicity, all `.o` files are linked to all test binaries, so all of them are regenerated if any `.h` or `.cpp` file changes, making the linking process a bit slow.

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
However, the conversion does not run automatically before compiling, you need to run it yourself, if you update the header file.

Add your sip file to `sip/recsys.sip` to include it in python compilation:

.. code-block:: sip

    %Include src/models/MyNewModel.sip

Then reinstall alpenglow:

.. code-block:: bash

   pip install --upgrade --force-reinstall --no-deps .

Now the new model is available in python:

.. code-block:: python
    :emphasize-lines: 2

    import alpenglow.Getter as rs
    my_new_model = rs.MyNewModel()
    rd = rs.RecDat()
    rd.time = 0
    rd.user = 10
    rd.item = 3
    my_new_model.prediction(rd)

Constructor parameters
----------------------

The constructor parameters are organized into a struct for each class, that has the same name as the class, appended `Parameters`.
To add a parameter named `fading_factor`, extend the header file like that:

.. code-block:: cpp
   :emphasize-lines: 1-3,9-11,14

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
    :emphasize-lines: 2-4

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
   :emphasize-lines: 2

    import alpenglow.Getter as rs
    my_new_model = rs.MyNewModel(fading_factor=0.8)
    rd = rs.RecDat()
    rd.time = 0
    rd.user = 10
    rd.item = 3
    my_new_model.prediction(rd)

Updater for the model
---------------------

The updater class, that performs the incremental update, must implement interface :py:class:`alpenglow.cpp.Updater`.  The following is the minimal implementation.

File `cpp/src/main/models/MyNewModelUpdater.h`:

.. code-block:: cpp

    #ifndef MY_NEW_MODEL_UPDATER_H
    #define MY_NEW_MODEL_UPDATER_H
    
    //SIP_AUTOCONVERT
    
    #include "../general_interfaces/Updater.h"
    #include "MyNewModel.h"
    
    class MyNewModelUpdater: public Updater{
      public:
        void update(RecDat* rec_dat) override;
        void set_model(MyNewModel* model){
          model_ = model;
        }
      private:
        MyNewModel* model_ = NULL;
    };
    
    #endif /* MY_NEW_MODEL_UPDATER_H */

File `cpp/src/main/models/MyNewModelUpdater.cpp`:

.. code-block:: cpp

    #include "MyNewModelUpdater.h"
    
    void MyNewModelUpdater::update(RecDat* rec_dat){
      return; //TODO perform incremental update here
    }

Declare the updater as a friend of the model class, so the updater can update the private state fields of the model:

.. code-block:: cpp
    :emphasize-lines: 5

    class MyNewModel
      : public Model
    {
      // ...
      friend class MyNewModelUpdater;
    };

Normally the unit test for the model and the updater is implemented as a common test.  Extend the unit test of the model:

.. code-block:: cpp
    :emphasize-lines: 1,4-5

    #include "../../main/models/MyNewModelUpdater.h"
    TEST_F(TestMyNewModel, test){
      // ...
      MyNewModelUpdater updater;
      updater.set_model(&model);
    }

Compile with `scons`, run the test, then generate sip file:

.. code-block:: bash

    sip/scripts/header2sip cpp/src/main/models/MyNewModelUpdater.h overwrite

Add the sip file to `sip/recsys.sip`

.. code-block:: sip

    %Include src/models/MyNewModelUpdater.sip

Reinstall the python module:

.. code-block:: bash

    pip install --upgrade --force-reinstall --no-deps .

Now the updater is available in python:

.. code-block:: python
   :emphasize-lines: 2

    import alpenglow.Getter as rs
    my_new_model = rs.MyNewModel(fading_factor=0.8)
    my_new_updater = rs.MyNewModelUpdater()
    my_new_updater.set_model(my_new_model)
    rd = rs.RecDat()
    my_new_updater.update(rd) #does nothing (yet)

Similarly, to create a logger and log some statistics about your model, create a class that implements interface `alpenglow.cpp.Logger`.

Add logic to the model and the updater
--------------------------------------

We implement a fading popularity model, that computes item popularity discounting exponentially in time.

.. code-block:: cpp

    TEST_F(TestMyNewModel, test){
      MyNewModelParameters model_params;
      model_params.fading_factor = 0.5;
      MyNewModel model(&model_params);
      MyNewModelUpdater updater;
      updater.set_model(&model);

      RecDat rec_dat;
      rec_dat.time=0;
      rec_dat.user=1;
      rec_dat.item=2;

      EXPECT_EQ(0,model.prediction(&rec_dat));

      rec_dat.time=1;
      rec_dat.item=2;
      updater.update(&rec_dat);
      EXPECT_EQ(1,model.prediction(&rec_dat));
      rec_dat.item=3;
      EXPECT_EQ(0,model.prediction(&rec_dat));

      rec_dat.time=2;
      rec_dat.item=2;
      EXPECT_DOUBLE_EQ(0.5,model.prediction(&rec_dat));
      rec_dat.item=3;
      EXPECT_EQ(0,model.prediction(&rec_dat));
      rec_dat.item=2;
      updater.update(&rec_dat);
      EXPECT_DOUBLE_EQ(1+0.5,model.prediction(&rec_dat));
    }

Now this test fails naturally. We implement the model:

.. code-block:: cpp

    class MyNewModel
      : public Model
    {
        // ...
        std::vector<double> scores_;
        std::vector<double> times_;
        // ...
    };

.. code-block:: cpp

   double MyNewModel::prediction(RecDat* rec_dat){
     int item = rec_dat->item;
     if (scores_.size() <= item) return 0;
     double time_diff = rec_dat->time-times_[item];
     return scores_[item]*std::pow(fading_factor_,time_diff);
   }

And the updater:

.. code-block:: cpp

    void MyNewModelUpdater::update(RecDat* rec_dat){
      int item = rec_dat->item;
      int time = rec_dat->time;
      if (item >= model_->scores_.size()) {
        model_->scores_.resize(item+1,0);
        model_->times_.resize(item+1,0);
      }
      double time_diff = time-model_->times_[item];
      model_->scores_[item]*=std::pow(model_->fading_factor_,time_diff);
      model_->scores_[item]+=1;
      model_->times_[item]=time;
    }

After recompiling with `scons`, the test passes.  These modifications are irrelevant for the sip files, but the python package needs to be reinstalled.

Creating an experiment using the new model
------------------------------------------

To create a preconfigured experiment using the new class, inherit from :py:class:`alpenglow.OnlineExperiment` and implement `_config()`.  Create file `python/alpenglow/experiments/MyNewExperiment.py`:

.. code-block:: python
    import alpenglow.Getter as rs
    import alpenglow as prs
    
    class MyNewExperiment(prs.OnlineExperiment):
        """Recommends the most popular item from the set of items seen so far,
           discounting exponentially by time.
        """
    
        def _config(self, top_k, seed):
            model = rs.MyNewModel(**self.parameter_defaults(
              fading_factor=0.8
            ))
            updater = rs.MyNewModelUpdater()
            updater.set_model(model)
    
            return (model, updater, [], [])

Append the new class to `python/alpenglow/experiments/__init__.py`:

.. code-block:: python

   from .MyNewExperiment import *

Create the corresponding integration test in `python/test_alpenglow/experiments/test_MyNewExperiment.py`:

.. code-block:: python
    import alpenglow as prs
    import alpenglow.Getter as rs
    import alpenglow.experiments
    import pandas as pd
    import math
    
    
    class TestMyNewExperiment:
        def test_MyNewExperiment(self):
            data = pd.read_csv(
                "python/test_alpenglow/test_data_4",
                sep=' ',
                header=None,
                names=['time', 'user', 'item', 'id', 'score', 'eval']
            )
            experiment = alpenglow.experiments.MyNewExperiment(
                top_k=100,
                seed=254938879,
                fading_factor=0.9
            )
            popRankings = experiment.run(data, verbose=True, exclude_known=True)
            print(list(popRankings["rank"].fillna(101)))
            assert popRankings.top_k == 100
            desired_ranks = [] #TODO
            assert list(popRankings["rank"].fillna(101)) == desired_ranks

Reinstall the python package and all the tests using `pytest` or only the new
test using the following command:

.. code-block:: bash

    pytest python/test_alpenglow/experiments/test_MyNewExperiment.py

The test will fail, but it will print the ranks produced by the model.  It
would be very time consuming to check whether all values are valid, but simple
errors (e.g. all values are 101 if the `set_model()` call is missing) might be
obvious.  If all seems to be fine, then copy the actual output to the expected
output field.  This way the test will catch unintentional modifications of the
logic of the model.

Now the new experiment is available in python, using similar code to the test.

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
