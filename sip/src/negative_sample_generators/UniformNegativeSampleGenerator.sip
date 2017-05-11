struct UniformNegativeSampleGeneratorParameters{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
    double negativeRate; 
    bool initializeAll;
    int maxItem;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator{
    %TypeHeaderCode
    #include "negative_sample_generators/UniformNegativeSampleGenerator.h"
    %End
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* params);
    void setTrainMatrix(SpMatrix* trainMatrix_);
    void setItems(std::vector<int>* items_);
    void setItems(VectorInt);
    %MethodCode
        sipCpp->setItems(&(a0->vec));
    %End
    //vector<int>* generate(RecDat * recDat);
    bool selfTest();
};
