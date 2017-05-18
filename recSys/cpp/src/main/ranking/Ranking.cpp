#include "Ranking.h"

int RankComputer::get_rank(RecDat* rec_dat){
  double score = recommender_->prediction(rec_dat);      
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
    double fake_score=recommender_->prediction(&fake_rec_dat);
    if(std::isnan(fake_score) or std::isinf(fake_score)) return (top_k_+3);
    if(fake_score > score){ rank++; }
    else if (fake_score == score) same_scored_itemnum++;
    if(rank == top_k_) break;
  }
  if(same_scored_itemnum!=0) rank+=random_.get(same_scored_itemnum+1);
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
      item = popularity_sorted_container_->getItem(itemlist_index_);
    }
    itemlist_index_++;
  } while (train_matrix_->get(fake_rec_dat->user,item)>=1 or item==itemlist_positive_item_);
  fake_rec_dat->item = item;
  return true;
}

//void TopListCreator::set_parameters(TopListCreatorParameters * parameters){
//  recommender = parameters->recommender;
//  items = parameters->items;
//  train_matrix = parameters->train_matrix;
//  top_k = parameters->top_k;
//}
//
//RecMap *  TopListCreator::getTopRecommendation(RecDat * recDat){
//  recommend(recDat);
//  sortTopK();
//  normTopK();
//  createTopKMap();
//  return &recMap;
//}
//
//void TopListCreator::recommend(RecDat * recDat){
//  rec.clear();
//  RecDat _recDat = *recDat;
//  for(uint ii=0; ii<items->size(); ii++){
//    int item = (*items)[ii];
//    if(train_matrix->get(recDat->user,item)<1) {
//      _recDat.item = item;
//      double score=recommender->prediction(&_recDat);
//      if(score != 0) rec.push_back(make_pair(item,score));
//    }
//  }
//}
//
//void TopListCreator::sortTopK(){
//  if((int)rec.size() > top_k) partial_sort (rec.begin(), rec.begin()+top_k, rec.end(),sortPairDescendingBySecond<int>);
//  else sort(rec.begin(),rec.end(),sortPairDescendingBySecond<int>);
//}
//
//void TopListCreator::normTopK(){
// for(uint ii=0 ; (ii<rec.size() && ii<(uint)top_k); ii++ ){
//  rec[ii].second=1/(double)(ii+1); 
// } 
//}
//
//void TopListCreator::createTopKMap(){
//  RecVector :: iterator ir = rec.end();
//  if((int)rec.size()>top_k) ir = rec.begin()+top_k;
//  recMap.clear();
//  copy(rec.begin(), ir,inserter(recMap, recMap.begin()));
//  rec.clear(); 
//}

