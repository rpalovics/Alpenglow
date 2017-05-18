#include "DistributionLogger.h"

//void DistributionLogger::set_model(Model* model_){
//  this->model=model_;
//}
//
//DistributionLogger::~DistributionLogger(){
//  // ofstream myfile;
//  // myfile.open(file_name);
//  // myfile << "vege van" << endl;  
//  // RecDat* example = new RecDat();
//  // example->user=1;
//  // example->item=5;
//  // example->score=0.0;
//  // myfile << this->model->prediction(example)<<endl;
//  // myfile << this->seen_items.size()<<endl;
//  // myfile << this->seen_users.size()<<endl;
//  // myfile.close();
//  
//  set<int> comp_users;
//
//  std::random_device rd;
//  std::minstd_rand gen(rd());
//  std::uniform_int_distribution<> dis(0, seen_users.size()-1);
//
//  while(comp_users.size() < user_number){
//    auto it = this->seen_users.begin();
//    int r_offset = dis(gen);
//    advance(it,r_offset);
//    comp_users.insert(*it);
//  }
//  ofstream myfile;
//  myfile.open (file_name);
//  for(auto u : comp_users){
//    float sum=0;
//    RecDat query;
//    query.user=u;
//    for(auto i : this->seen_items){
//      query.item=i;
//      sum+=this->model->prediction(&query);
//    }
//    float avg = sum/(this->seen_items.size());
//
//    float varsum=0;
//    for(auto i : this->seen_items){
//      query.item=i;
//      varsum+=pow(this->model->prediction(&query)-avg,2);
//    }
//    float var = pow(varsum/(this->seen_items.size()),1.0/2.0);
//    for(auto i : this->seen_items){
//      query.item=i;
//      float gpred = (this->model->prediction(&query)-avg)/var;
//      myfile << gpred << endl;
//    }
//  }
//  myfile.close();
//}
//
//void DistributionLogger::run(RecDat* rec_dat){
//	this->seen_items.insert(rec_dat->item);
//	this->seen_users.insert(rec_dat->user);
//}
//
//void DistributionLogger::set_parameters(DistributionLoggerParameters * parameters){
//  file_name=parameters->file_name;
//	user_number=parameters->user_number;
//}
