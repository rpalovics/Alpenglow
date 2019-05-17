
struct TimeframeDataGeneratorParameters {
  %TypeHeaderCode
  #include "data_generators/TimeframeDataGenerator.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.TimeframeDataGenerator`. See documentation there.
  %End
  int timeframe_length;
};
class TimeframeDataGenerator : public DataGenerator, public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "data_generators/TimeframeDataGenerator.h"
  %End
  public:
    TimeframeDataGenerator(TimeframeDataGeneratorParameters* params);
    RecommenderData* generate_recommender_data(RecDat*);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

