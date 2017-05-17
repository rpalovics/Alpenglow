struct UniformNegativeSampleGeneratorParameters{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
    double negativeRate; 
    bool initializeAll;
    int maxItem;
    double negative_rate; 
    bool initialize_all;
    int max_item;
    bool filter_repeats;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* params);
    void set_train_matrix(SpMatrix* train_matrix);
    void set_items(std::vector<int>* items);
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    bool self_test();
    void setTrainMatrix(SpMatrix* train_matrix_);
    void setItems(std::vector<int>* items_);
    void setItems(VectorInt);
    %MethodCode
        sipCpp->setItems(&(a0->vec));
    %End
    //vector<int>* generate(RecDat * recDat);
    bool selfTest();
};
