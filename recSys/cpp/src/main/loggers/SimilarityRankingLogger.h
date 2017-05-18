#ifndef SIMILARITY_RANKING_LOGGER
#define SIMILARITY_RANKING_LOGGER

#include <vector>
#include "../models/SimilarityModel.h"
#include "Logger.h"
#include "../recommender_data/AttributeContainer.h"

struct SimilarityRankingLoggerParameters{
  int magic_user;
  int top_k;
  string file_name; 
};

class SimilarityRankingLogger : public Logger{
  public:
    SimilarityRankingLogger(SimilarityRankingLoggerParameters* params){
      magic_user_ = params->magic_user;
      top_k_ = params->top_k;
      file_name_ = params->file_name;
      output_file_.open(file_name_.c_str());
      model_ = NULL;
      session_container_ = NULL;
      items_ = NULL;
      clicked_recdat_ = NULL;
      old_session_ = -1;
    } 
    ~SimilarityRankingLogger(){}
    //void init(){}
    void run(RecDat* rec_dat){
      if(beginning_of_session(rec_dat)){
        clicked_recdat_ = rec_dat;
      }
      else if(inside_of_session(rec_dat)){
        if(rec_dat->eval!=0 && clicked_recdat_->item != rec_dat->item){ //az egy sorozatban ismetlodo kattintasokat kiszurjuk, es ha az eval==0, nincs kiertekeles
          int rank = compute_rank(rec_dat);
          write_out_rank(rec_dat, rank);
        }
      }
    }
    bool beginning_of_session(RecDat* rec_dat){
      if(magic_user_!=-1){
        return rec_dat->user==magic_user_ && clicked_recdat_==NULL;
      }
      if(session_container_!=NULL){
        int session = session_container_->get_attribute(rec_dat->id);
        if(session != old_session_){
          old_session_ = session; //changes state, the function is non repeatable
          return true;
        } else {
          return false;
        }
      }
      return false;
    }
    bool inside_of_session(RecDat* rec_dat){ //felteve, hogy nem session eleje
      if(magic_user_!=-1){
        return magic_user_==rec_dat->user;
      }
      if(session_container_!=NULL){
        int session = session_container_->get_attribute(rec_dat->id);
        return old_session_ == session;
      }
      return false;
    }
    int compute_rank(RecDat* rec_dat){
      int clicked_item = clicked_recdat_->item;
      int item = rec_dat->item;
      double similarity = model_->similarity(clicked_item, item);      
      if(std::isnan(similarity) or std::isinf(similarity)) return (top_k_+3);
      if(similarity==0) return top_k_+2;
      int rank = 0;
      //int same_similarity_itemnum = 0;
      for(uint ii=0; ii<items_->size(); ii++){ 
        int fake_item = items_->at(ii);
        if(fake_item!=item and fake_item!=clicked_item) {
          double fake_similarity = model_->similarity(clicked_item,fake_item);
          if(fake_similarity > similarity){
            rank++;
          }
          //else if (fake_similarity == similarity) same_similarity_itemnum++;
          if(rank >= top_k_) break;
        }
      }
      //if(same_similarity_itemnum!=0) rank+=random_.get(same_similarity_itemnum+1);
      //rank=min(top_k_,rank);
      return rank;
    }
    void write_out_rank(RecDat* rec_dat, int rank){
        output_file_ << (int) rec_dat->time << " " << rank << " " << model_->similarity(clicked_recdat_->item, rec_dat->item) << " " << rec_dat->user << " " << rec_dat->item << " " << endl;
    }
    void set_model(SimilarityModel* model){ model_ = model;} 
    void set_session_container(AttributeContainer<int>* session_container){session_container_ = session_container; }
    void set_items(vector<int>* items){ items_ = items;} 
    bool self_test(){
      bool ok = Logger::self_test();
      if(top_k_ < 0){
        ok = false;
        cerr << "SimilarityRankingLogger::top_k is negative." << endl;
      }
      if(model_ == NULL){
        ok = false;
        cerr << "SimilarityRankingLogger::model_ is not set." << endl;
      }
      if(session_container_ == NULL && magic_user_ == -1){
        ok = false;
        cerr << "SimilarityRankingLogger::session_container and magic_user is not set, cant distinguish sessions." << endl;
      }
      if(items_ == NULL){
        ok = false;
        cerr << "SimilarityRankingLogger::items_ is not set." << endl;
      }
      return ok;
    }
  private:
    //parameters
    int top_k_;
    int magic_user_;
    string file_name_;
    SimilarityModel* model_;
    AttributeContainer<int>* session_container_;
    vector<int>* items_;
    //state
    ofstream output_file_;
    RecDat* clicked_recdat_;
    int old_session_;
};

#endif
