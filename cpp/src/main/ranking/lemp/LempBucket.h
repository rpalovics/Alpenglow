#ifndef LEMP_BUCKET_H
#define LEMP_BUCKET_H

#include <vector>

using namespace std;

struct LempBucket{
  vector<int> item_ids_;
  vector<double> item_lengths_;
  vector<vector<double>> item_directions_;

  void insert_item(int id, vector<double> *factor);
  void erase_item(int id);
  int size() const;
  double get_bucket_max() const;
  LempBucket split(int num_to_split);

  bool operator<(const LempBucket &comp) const {
      if (get_bucket_max() < comp.get_bucket_max()) return true;
      else return false;
  }

  struct less_comparator
  {
      bool operator() (const LempBucket *b1, const LempBucket *b2) const {
          return b1->get_bucket_max() > b2->get_bucket_max();
      }
  };

  vector<pair<int,double>> get_scores(vector<double> *user_factor);
};

#endif /* LEMP_BUCKET_H */
