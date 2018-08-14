#include "SamplingDataGenerator.h"

RecommenderData* SamplingDataGenerator::generate_recommender_data(RecDat*){
  local_recommender_data_.clear();
  int random_index_boundary = recommender_data_iterator_->get_counter()+1;
  vector<RecDat>* train_data = local_recommender_data_.get_rec_data();
  for(int i=0;i<number_of_samples_;i++){
    int index_of_sample = 0;
    if(distribution_=="uniform"){
      index_of_sample=random_->get(random_index_boundary);
    } else if (distribution_=="linear"){
      index_of_sample=random_->get_linear(random_index_boundary);
    } else if (distribution_=="arctg"){
      index_of_sample=random_index_boundary-1-random_->get_arctg(y_, random_index_boundary); //kivonas oka: ez az eloszlas csokkeno, nekunk novekvo kell
    } else if (distribution_=="geometric"){
      index_of_sample=random_index_boundary-1-random_->get_geometric(geometric_param_, random_index_boundary); //kivonas oka: ez az eloszlas csokkeno, nekunk novekvo kell
    } 
    RecDat* sample = recommender_data_iterator_->get(index_of_sample);
    train_data->push_back(*sample);
  }
  if (!local_recommender_data_.initialize()) throw 0;
  return &local_recommender_data_;
}
