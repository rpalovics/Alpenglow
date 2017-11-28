#ifndef FACTORS_LEMP_CONTAINER_H
#define FACTORS_LEMP_CONTAINER_H

#include "LempContainer.h"
#include "../../utils/Factors.h"

using namespace std;

class FactorsLempContainer : public LempContainer{
public:
  FactorsLempContainer(Factors *item_factors, int bucket_size);
  void reinitialize(Factors *item_factors, int bucket_size);
  void reinitialize(Factors *item_factors){
    reinitialize(item_factors, bucket_size_);
  }
protected:
  vector<tuple<int,double,vector<double>>> factor_to_items(Factors *factor);
  vector<tuple<int,double,vector<double>>> get_factor_items() override;
  vector<double> get_factor_item(int id) override;

  Factors *item_factors_;

  FRIEND_TEST(TestLempContainer, insert_remove);
  FRIEND_TEST(TestLempContainer, find_closest_bucket);
  FRIEND_TEST(TestLempContainer, split_on_insert);
  FRIEND_TEST(TestLempContainer, update);
  FRIEND_TEST(TestLempContainer, scheduled_update);
};

#endif