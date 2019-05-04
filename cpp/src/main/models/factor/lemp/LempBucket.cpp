#include <algorithm>
#include <limits>
#include "LempBucket.h"
#include "../../../utils/Util.h"

vector<pair<int,double>> LempBucket::get_scores(vector<double> *user_factor){
  vector<pair<int,double>> scores;
  scores.reserve(size());
  for(int i=0; i<size(); i++){
    int &id = item_ids_[i];
    vector<double> &direction = item_directions_[i];
    double &length = item_lengths_[i];
    
    double score = Util::scalar_product(&direction, user_factor);
    score *= length;
    scores.push_back({id,score});
  }
  return scores;
}

void LempBucket::insert_item(int id, vector<double> *factor){
  double norm = Util::norm(factor);
  vector<double> direction = vector<double>(*factor);
  Util::multiply_vector(1/norm, &direction);

  //find insert position
  int index = std::upper_bound(
    item_lengths_.begin(),
    item_lengths_.end(),
    norm,
    [](double a, double b) { return a >= b; }
  ) - item_lengths_.begin();
  
  item_lengths_.insert(item_lengths_.begin()+index, norm);
  item_directions_.insert(item_directions_.begin()+index, direction);
  item_ids_.insert(item_ids_.begin()+index, id);
}

void LempBucket::erase_item(int id){
  //find erase position
  int index = std::find(
    item_ids_.begin(),
    item_ids_.end(),
    id
  ) - item_ids_.begin();

  item_lengths_.erase(item_lengths_.begin()+index);
  item_directions_.erase(item_directions_.begin()+index);
  item_ids_.erase(item_ids_.begin()+index);
}

int LempBucket::size() const {
  return item_lengths_.size();
}

LempBucket LempBucket::split(int num){
  LempBucket new_bucket;

  new_bucket.item_ids_ = vector<int>(item_ids_.end()-num, item_ids_.end());
  new_bucket.item_lengths_ = vector<double>(item_lengths_.end()-num, item_lengths_.end());
  new_bucket.item_directions_ = vector<vector<double>>(item_directions_.end()-num, item_directions_.end());

  int new_size = size()-num;
  item_ids_.resize(new_size);
  item_lengths_.resize(new_size);
  item_directions_.resize(new_size);

  return new_bucket;
}

double LempBucket::get_bucket_max() const {
  if(size() > 0){
    return item_lengths_[0];
  } else {
    return std::numeric_limits<double>::lowest();
  }
}
