#include "RankComputer.h"

#include <utility>

int RankComputer::get_rank(RecDat* rec_dat){
  // Model filters aren't handled yet.
  // If the model is an instance ToplistFromRankingScoreRecommender,
  // we evaluate it as RankingScoreIteratorProvider.
  if ( experiment_environment_!=NULL
       && !experiment_environment_->is_item_existent(rec_dat->item)){
    return top_k_+5;
  }
  if(toplist_model_!=NULL && tfrsr_model_==NULL && model_filter_==NULL){
    return get_rank_toplist_model(rec_dat);
  } else if(ranking_model_!=NULL && model_filter_==NULL){
    return get_rank_ranking_model(rec_dat);
  } else {
    return get_rank_bruteforce(rec_dat);
  }
}

int RankComputer::get_rank_toplist_model(RecDat* rec_dat){
  vector<pair<int,double>> list = toplist_model_->get_top_list(rec_dat->user, top_k_, train_matrix_);
  int i=0;
  for(auto t : list){
    int item = get<0>(t);
    if(item == rec_dat->item){
      return i;
    }
    i++;
  }
  return top_k_;
}

int RankComputer::get_rank_ranking_model(RecDat* rec_dat){
  double score = model_->prediction(rec_dat);
  if(std::isnan(score) or std::isinf(score)) return (top_k_+3);
  if(score==0) return top_k_+2;

  RankingScoreIterator *iterator = ranking_model_->get_ranking_score_iterator(rec_dat->user);

  if(iterator == NULL){
    return get_rank_bruteforce(rec_dat);
  }

  const map<int,double> *seen_items = train_matrix_->get(rec_dat->user);
  map<int,double> empty_seen_items;
  if(seen_items == NULL){
    seen_items = &empty_seen_items;
  }

  int better_items_num = 0;
  int equal_items_num = 0;

  if(score < 0){
    //even unitialized items are better
    //however we only count them if they don't appear in the matrix
    for(uint i=iterator->unique_items_num(); i<items_->size(); i++){
      const int& item = (*items_)[i];
      if(item != rec_dat->item && seen_items->find(item) == seen_items->end()){
        better_items_num += 1;
      }
    }
    }

  while(iterator->has_next(score)){
    double item_score;
    int item_id;
    tie(item_id, item_score) = iterator->get_next();
    if(item_id == rec_dat->item || seen_items->find(item_id) != seen_items->end()){
    } else if(item_score > score){
      better_items_num++;
    } else if (item_score == score){
      equal_items_num++;
    }

    if(better_items_num == top_k_){
      break;
    }
  }

  if(equal_items_num!=0){
    better_items_num += random_.get(equal_items_num+1);
  }

  better_items_num = min(top_k_,better_items_num);
  return better_items_num;
}

int RankComputer::get_rank_bruteforce(RecDat* rec_dat){
  double score = model_->prediction(rec_dat);      
  if(std::isnan(score) or std::isinf(score)) return (top_k_+3);
  if(score==0) return top_k_+2;
  if(model_filter_!=NULL){
    model_filter_->run(rec_dat);
    if(!model_filter_->active(rec_dat)) return top_k_+4;
  }
  RecDat fake_rec_dat = *rec_dat;
  int rank = 0;
  int same_scored_itemnum = 0;
  itemlist_init(rec_dat);
  while(itemlist_next(&fake_rec_dat)){
    double fake_score=model_->prediction(&fake_rec_dat);
    if(std::isnan(fake_score) or std::isinf(fake_score)) return (top_k_+3);
    if(fake_score > score) rank++;
    else if (fake_score == score) same_scored_itemnum++;
    if(rank == top_k_) break;
  }
  if(same_scored_itemnum!=0){
    rank+=random_.get(same_scored_itemnum+1);
  }
  rank=min(top_k_,rank);
  return rank;
}

void RankComputer::itemlist_init(RecDat* rec_dat){
  itemlist_index_ = 0;
  itemlist_positive_item_ = rec_dat->item;
  if(model_filter_!=NULL){
    itemlist_ = model_filter_->get_personalized_items(rec_dat->user);
  }
  if(itemlist_!=NULL){
    itemlist_max_index_ = itemlist_->size();
  } else {
    itemlist_max_index_ = popularity_sorted_container_->size();
  }
}

bool RankComputer::itemlist_next(RecDat* fake_rec_dat){
  int item = 23;
  do {
    if(itemlist_index_>=itemlist_max_index_) return false;
    if(itemlist_!=NULL){
      item = itemlist_->at(itemlist_index_).first;
    } else {
      item = popularity_sorted_container_->get_item(itemlist_index_);
    }
    itemlist_index_++;
  } while (train_matrix_->get(fake_rec_dat->user,item)>=1 or item==itemlist_positive_item_);
  fake_rec_dat->item = item;
  return true;
}

