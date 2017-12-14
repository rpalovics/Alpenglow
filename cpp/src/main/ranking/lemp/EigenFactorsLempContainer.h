#ifndef EIGEN_FACTORS_LEMP_CONTAINER_H
#define EIGEN_FACTORS_LEMP_CONTAINER_H

#include "LempContainer.h"
#include "../../utils/EigenFactors.h"

using namespace std;

class EigenFactorsLempContainer : public LempContainer{
public:
  EigenFactorsLempContainer(EigenFactors *item_factors, int bucket_size);
  void reinitialize(EigenFactors *item_factors, int bucket_size);
protected:
  vector<tuple<int,double,vector<double>>> factor_to_items(EigenFactors *factor);
  vector<tuple<int,double,vector<double>>> get_factor_items() override;
  vector<double> get_factor_item(int id) override;

  EigenFactors *item_factors_;

  FRIEND_TEST(TestLempContainer, insert_remove);
  FRIEND_TEST(TestLempContainer, find_closest_bucket);
  FRIEND_TEST(TestLempContainer, split_on_insert);
  FRIEND_TEST(TestLempContainer, update);
  FRIEND_TEST(TestLempContainer, scheduled_update);
};

#endif