#include "DistributionLogger.h"

//void DistributionLogger::setModel(Model* model_){
//  this->model=model_;
//}
//
//DistributionLogger::~DistributionLogger(){
//  // ofstream myfile;
//  // myfile.open(fileName);
//  // myfile << "vege van" << endl;  
//  // RecDat* example = new RecDat();
//  // example->user=1;
//  // example->item=5;
//  // example->score=0.0;
//  // myfile << this->model->prediction(example)<<endl;
//  // myfile << this->seenItems.size()<<endl;
//  // myfile << this->seenUsers.size()<<endl;
//  // myfile.close();
//  
//  set<int> compUsers;
//
//  std::random_device rd;
//  std::minstd_rand gen(rd());
//  std::uniform_int_distribution<> dis(0, seenUsers.size()-1);
//
//  while(compUsers.size() < userNumber){
//    auto it = this->seenUsers.begin();
//    int rOffset = dis(gen);
//    advance(it,rOffset);
//    compUsers.insert(*it);
//  }
//  ofstream myfile;
//  myfile.open (fileName);
//  for(auto u : compUsers){
//    float sum=0;
//    RecDat query;
//    query.user=u;
//    for(auto i : this->seenItems){
//      query.item=i;
//      sum+=this->model->prediction(&query);
//    }
//    float avg = sum/(this->seenItems.size());
//
//    float varsum=0;
//    for(auto i : this->seenItems){
//      query.item=i;
//      varsum+=pow(this->model->prediction(&query)-avg,2);
//    }
//    float var = pow(varsum/(this->seenItems.size()),1.0/2.0);
//    for(auto i : this->seenItems){
//      query.item=i;
//      float gpred = (this->model->prediction(&query)-avg)/var;
//      myfile << gpred << endl;
//    }
//  }
//  myfile.close();
//}
//
//void DistributionLogger::run(RecDat* recDat){
//	this->seenItems.insert(recDat->item);
//	this->seenUsers.insert(recDat->user);
//}
//
//void DistributionLogger::setParameters(DistributionLoggerParameters * parameters){
//  fileName=parameters->fileName;
//	userNumber=parameters->userNumber;
//}
