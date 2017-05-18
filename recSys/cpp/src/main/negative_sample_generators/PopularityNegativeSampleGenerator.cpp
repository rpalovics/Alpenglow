#include "PopularityNegativeSampleGenerator.h"

vector<int> * PopularityNegativeSampleGenerator::generate(RecDat* rec_dat){
  samples.clear();
  //create map of unavailable items
  vector<int> unavailableItemsMap;
  for(int i=0;i<items->size();i++){ //set itemMap size
    if(unavailableItemsMap.size()<items->at(i)+1) unavailableItemsMap.resize(items->at(i)+1,false);
  }
  //get sum of popularity distribution
  int sumOfPopularity = 0;
  for(int i=0;i<items->size();i++){
    int popularity = pop_container->get(items->at(i));
    sumOfPopularity+=popularity;
  }
  //remove positive items from availability map
  MatrixRow* userActivity = train_matrix->get(rec_dat->user);
  for(RowIterator it = userActivity->begin();it!=userActivity->end();it++){
    int positiveItem = it->first;
    unavailableItemsMap[positiveItem]=true;
    sumOfPopularity-= pop_container->get(positiveItem);
  }
  //generate negative samples
  int numOfChosen = 0;
  for(numOfChosen=0;numOfChosen<negativeRate && sumOfPopularity>0;numOfChosen++){
    int randIndex = random(sumOfPopularity); //index negative sample in the virtual popularity weighted item array, the index of the first element is 1
    randIndex++;
    int itemIndex=-1; //index of actual item in items
    int item; //items[itemIndex]
    int buffer = 0; //sum of pop of available items in items[0,itemIndex]
    while (buffer < randIndex){ //when buffer is equal or larger, we found our sample
      itemIndex++;
      item = items->at(itemIndex);
      if(!unavailableItemsMap[item]) buffer+=pop_container->get(item);
    }
    samples.push_back(item);
    sumOfPopularity-=pop_container->get(item);
    unavailableItemsMap[item]=true;
  }
  return &samples;
}
