#include "UniformNegativeSampleGenerator.h"

void UniformNegativeSampleGenerator::setParameters(UniformNegativeSampleGeneratorParameters* parameters){
  items = NULL;
  trainMatrix = NULL;
  negativeRate = parameters->negativeRate; 
  initializeAll=parameters->initializeAll;
  if(initializeAll){
    maxItem=parameters->maxItem;
    items=new vector<int>(maxItem+1);
    for(int i=0;i<items->size();i++){items->at(i)=i;}
  }
}

vector<int>* UniformNegativeSampleGenerator::generate(RecDat * recDat){
  if(!filterRepeats){
    int learnt = 0;
    samples.clear();
    int userActivity = trainMatrix->rowSize(recDat->user);
    while(learnt < negativeRate && learnt<(int)items->size()-userActivity-1){
      int item = items->at((int)(rand()/(RAND_MAX+1.0)*(items->size())));
      if(!trainMatrix->hasValue(recDat->user,item) && item!=recDat->item){
        learnt++;
        samples.push_back(item);
      }
    }  
    return &samples;
  } else {
    int number_of_generated = 0;
    int available = items->size();
    samples.clear();
    while(number_of_generated < negativeRate && available>0){
      int idx = ((int)(rand()/(RAND_MAX+1.0)*available));
      int item = items->at(idx);
      if(!trainMatrix->hasValue(recDat->user,item) && item!=recDat->item){
        number_of_generated++;
        samples.push_back(item);
      }
      items->at(idx)=items->at(available-1);
      items->at(available-1) = item;
      available--;
    }  
    return &samples;
  }
}

