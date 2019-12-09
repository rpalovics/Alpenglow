Implementing a new model in C++
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

Now this test naturally fails. We implement the model:

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

To create a preconfigured experiment using the new class, inherit from
:py:class:`alpenglow.OnlineExperiment` and implement `_config()`.  Calling
`run()` for the new class will run the experiment, i.e. for each sample,
compute rank and then call the updater to update the model.  Create file
`python/alpenglow/experiments/MyNewExperiment.py`:

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
would be very time consuming to check whether all values are correct, but
simple errors (e.g. all values are 101 because the `set_model()` call is
missing) might be obvious.  If all seems to be fine, then copy the actual
output to the expected output field.  This way the test will catch
unintentional modifications of the logic of the model.

Now the new experiment is available in python, using similar code to the test.

Document your model
-------------------

To document the C++ clasees, use java-style documentation comments in the
header files.  Note that the comment describing the class is after the opening
bracket of the class declaration, and the comment that belongs to the function
is after the function declaration.

.. code-block:: cpp
  :emphasize-lines: 4-7,13-28

  class MyNewModel
    : public Model
  {
  /**
    Item popularity based model.  The popularity of the items fades
    in time exponentially.
  */
    public:
      MyNewModel(MyNewModelParameters* params){
        fading_factor_ = params->fading_factor;
      }
      double prediction(RecDat* rec_dat) override;
      /** 
        prediction(RecDat)
  
        Computes prediction score for the sample.  Uses only the time
        and item fields, the user is ignored.
  
        Parameters
        ----------
        rec_dat : RecDat*
            The sample.
  
        Returns
        -------
        double
            The prediction score.
     */
     // ...
  }

Then transform the comments by the header->sip converter, reinstall the python
package and regenerate the documentation.  The reinstallation step is
necessary, as the documentation generator acquires the documentation from the
installed alpenglow package.

.. code-block:: bash

    sip/scripts/header2sip cpp/src/main/models/MyNewModel.h overwrite
    pip install --upgrade --force-reinstall --no-deps .
    cd docs
    make dirhtml

The process is similar for the updater.  To document an experiment, add a
docstring (already shown in the example above).

Implement further functions of the Model interface
--------------------------------------------------

The `Model` interface provides 4 more functions to override, and the framework provides one:

.. code-block:: cpp

   //void add(RecDat* rec_dat) override; //not applicable in our case
   void write(ostream& file) override;
   void read(istream& file) override;
   void clear() override;
   bool self_test();

Function `add()` is called before gradient updates.  It notifies the model
about the existance of a user and an item, and its responsibility is the
(random) initialization of the model w.r.t. the item and user in the parameter.
As an example, consider the random initialization of factors in case of a
factor model.

Functions `write()` and `read()` implement serialization.  While serialization
possibilities are not complete in the framework, it is possible to write out
and read back models in `alpenglow.experiment.BatchFactorExperiment` and
`alpenglow.experiment.BatchAndOnlineExperiment`.  For details, see
:doc:`serialization`.

Function `clear()` must clear and reinitialize the model.

Function `self_test()` must check whether all components are properly set, the
parameters are sane etc.  The main goal is to prevent hard-to-debug
segmentation faults caused by missing `set_xxx()` calls. Note that
`self_test()` is not virtual, it is called by the framework for the appropriate
type and it is the functions responsibility to call `self_test()` of its
ancestors.

Here are the expanded testcases:

.. code-block:: cpp

    TEST_F(TestMyNewModel, test){
      // ...

      //read, write
      std::stringstream ss;
      model.write(ss);
      model.write(ss);
    
      MyNewModel model2(&model_params);
      model2.read(ss);
      EXPECT_DOUBLE_EQ(model.prediction(&rec_dat), model2.prediction(&rec_dat));
      MyNewModel model3(&model_params);
      model3.read(ss);
      EXPECT_DOUBLE_EQ(model.prediction(&rec_dat), model3.prediction(&rec_dat));
    
      //clear
      model.clear();
      
      for(int item : {0,1,2,3,4,5}){
        rec_dat.item=item;
        EXPECT_EQ(0,model.prediction(&rec_dat));
      }
    }

.. code-block:: cpp
    
    TEST_F(TestMyNewModel, self_test){
      MyNewModelParameters model_params;
      model_params.fading_factor = 0.5;
      MyNewModel model(&model_params);
      EXPECT_TRUE(model.self_test());
    
      model_params.fading_factor = 0;
      MyNewModel model2(&model_params);
      EXPECT_TRUE(model2.self_test());
    
      model_params.fading_factor = -0.2;
      MyNewModel model3(&model_params);
      EXPECT_FALSE(model3.self_test());
    }

And the implementations:

.. code-block:: cpp

    void MyNewModel::write(ostream& file){
      file << scores_.size() << " ";
      for (double score : scores_){
        file << score << " ";
      }
      file << times_.size() << " ";
      for (double time : times_){
        file << time << " ";
      }
    }
    void MyNewModel::read(istream& file){
      int scores_size;
      file >> scores_size;
      scores_.resize(scores_size);
      for (uint i=0;i<scores_.size();i++){
        file >> scores_[i];
      }
      int times_size;
      file >> times_size;
      times_.resize(times_size);
      for (uint i=0;i<times_.size();i++){
        file >> times_[i];
      }
    }
    void MyNewModel::clear(){
      scores_.clear();
      times_.clear();
    }

Normally `self_test()` is implemented in the header:

.. code-block:: cpp

   bool self_test() {
     bool ok = Model::self_test();
     if (fading_factor_<0) ok = false;
     return ok;
   }

Regenerate the sip file and reinstall the python package to make the new
functions available in python and visible for the online experiment framework.

Access common data
------------------

In the online experiment, the framework provides some common parameters and
statistics through class :py:class:`alpenglow.cpp.ExperimentEnvironment` (see
details there).  To access them, the class needs to implement interface
:py:class:`alpenglow.cpp.NeedsExperimentEnvironment`, so the online experiment
framework will set the `ExperimentEnvironment` object.

Typically such classes also implement :py:class:`Initializable`, asking the
framework to call their `autocalled_initialize()` function when the experiment
is already built (after the `set_xxx()` calls), and in that function, they copy
the pointers to the common objects.  See the example below.

.. code-block:: cpp

  #include "../general_interfaces/NeedsExperimentEnvironment.h"
  #include "../general_interfaces/Initializable.h"
  // ...
  class MyNewModel
    : public Model
    , public NeedsExperimentEnvironment
    , public Initializable
  {
    public:
      // ...
      void set_items(const vector<int>* items){ items_ = items; }
      bool self_test() {
        bool ok = Model::self_test();
        if (fading_factor_<0) ok = false;
        if (items_==NULL) ok=false;
        return ok;
      }
    private:
      bool autocalled_initialize(){
        if (items_ == NULL) { //items_ is not set
          if (experiment_environment_!=NULL){ //exp_env is available
            items_ = experiment_environment_->get_items();
          } else {
            return false; //can't set items
          }
        }
        return true;
      }
      const std::vector<int>* items_ = NULL;
      // ...
  };

We also need to update the unit test:

.. code-block:: cpp

  class TestMyNewModel : public ::testing::Test {
    public:
      vector<int> items;
      // ...
  };
  
  TEST_F(TestMyNewModel, test){
    // ...
    MyNewModel model(&model_params);
    model.set_items(&items);
    // ...
    items.push_back(2);
    updater.update(&rec_dat);
    // ...
  }


.. code-block:: cpp
  :emphasize-lines: 5,10,15,18-27

  TEST_F(TestMyNewModel, self_test){
    MyNewModelParameters model_params;
    model_params.fading_factor = 0.5;
    MyNewModel model(&model_params);
    model.set_items(&items);
    EXPECT_TRUE(model.self_test());
  
    model_params.fading_factor = 0;
    MyNewModel model2(&model_params);
    model2.set_items(&items);
    EXPECT_TRUE(model2.self_test());
  
    model_params.fading_factor = -0.2;
    MyNewModel model3(&model_params);
    model3.set_items(&items);
    EXPECT_FALSE(model3.self_test());
  
    model_params.fading_factor = 0.5;
    MyNewModel model4(&model_params);
    EXPECT_FALSE(model4.self_test());
  
    model_params.fading_factor = 0.5;
    MyNewModel model5(&model_params);
    ExperimentEnvironment expenv;
    model5.set_experiment_environment(&expenv);
    EXPECT_TRUE(model5.initialize());
    EXPECT_TRUE(model5.self_test());
  }

However, changing the test of `MyNewExperiment` is not necessary as the
framework automatically sets `experiment_environment_` and calls
`autocalled_initialize()`.  The alternative setting method, `set_items()` is
necessary for offline experiments where `exp_env` is not available and might be
useful in unit tests.

Make evaluation faster
----------------------

In the online experiment, the rank of the relevant item is computed by default
by comparing its score to the score of the other items one-by-one.  The
computation can halt when we already found more better items, than the rank
threshold, or when all items were compared to the relevant.  By processing the
items first that has higher score, we make the process faster.  This is the
goal of the interface :py:class:`alpenglow.cpp.RankingScoreIterator`. In
addition, in case of some model implementation, keeping an up-to-date toplist
is computationally easy. For these models, we can query the rank directly.
Such models should implement the interface
:py:class:`alpenglow.cpp.ToplistRecommender`.
