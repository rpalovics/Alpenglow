#include "PopularityNegativeSampleGenerator.h"

vector<int>* PopularityNegativeSampleGenerator::generate(RecDat* rec_dat){
  samples.clear();
  //create map of unavailable items
  vector<int> unavailable_items_map;
  for(uint i=0;i<items->size();i++){ //set item_map size
    if((int)unavailable_items_map.size()<items->at(i)+1) unavailable_items_map.resize(items->at(i)+1,false);
  }
  //get sum of popularity distribution
  int sum_of_popularity = 0;
  for(uint i=0;i<items->size();i++){
    int popularity = pop_container->get(items->at(i));
    sum_of_popularity+=popularity;
  }
  //remove positive items from availability map
  const MatrixRow* user_activity = train_matrix->get(rec_dat->user);
  for(auto it = user_activity->begin();it!=user_activity->end();it++){
    int positive_item = it->first;
    unavailable_items_map[positive_item]=true;
    sum_of_popularity-= pop_container->get(positive_item);
  }
  //generate negative samples
  int num_of_chosen = 0;
  for(num_of_chosen=0;num_of_chosen<negative_rate && sum_of_popularity>0;num_of_chosen++){
    int rand_index = random_(sum_of_popularity); //index negative sample in the virtual popularity weighted item array, the index of the first element is 1
    rand_index++;
    int item_index=-1; //index of actual item in items
    int item; //items[item_index]
    int buffer = 0; //sum of pop of available items in items[0,item_index]
    while (buffer < rand_index){ //when buffer is equal or larger, we found our sample
      item_index++;
      item = items->at(item_index);
      if(!unavailable_items_map[item]) buffer+=pop_container->get(item);
    }
    samples.push_back(item);
    sum_of_popularity-=pop_container->get(item);
    unavailable_items_map[item]=true;
  }
  return &samples;
}
