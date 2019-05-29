
struct UniformNegativeSampleGeneratorParameters {
  %TypeHeaderCode
  #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.UniformNegativeSampleGenerator`. See documentation there.
  %End
    double negative_rate; 
    bool filter_repeats;
    int seed;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator, public Initializable, public NeedsExperimentEnvironment {
  %TypeHeaderCode
  #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
  %End
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* parameters);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_items(std::vector<int>* items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End

    std::vector<int>* generate(RecDat* rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

