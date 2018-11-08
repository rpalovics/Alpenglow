#include "PopContainers.h"


void PopContainer::increase(int item){
  resize(item);
  popularities[item]++;
}

void PopContainer::reduce(int item){
  popularities[item]--;
}

void PopContainer::resize(int item){
  if((int)popularities.size()<item+1) popularities.resize(item+1);
}

int PopContainer::get(int item){ 
  if((int)popularities.size()<item+1) return 0;
  else return popularities[item];
}

void TopPopContainer::increase(int item){ 
  if((int)idxs.size()<item+1){
    idxs.resize(item+1,-1);
  }
  if(idxs[item]==-1){
    popularities.push_back(1);
    names.push_back(item);
    idxs[item]=popularities.size()-1;
  } else{
    popularities[idxs[item]]++;
    swap_up(item);
  } 
}

//void TopPopContainer::reduce(int item){
//  popularities[idxs[item]]--;
//  swap_down(item);
//}

pair <int,double> TopPopContainer::get(int idx){
   changed=false;
   pair <int,double> p;
   if(idx<(int)names.size()){
     p.first=names[idx];
     p.second=popularities[idx];
   }
   return p;
}

//pair <int,double> TopPopContainer::get_normed(int idx){
//   changed=false;
//   pair <int,double> p;
//   if(idx<(int)names.size()){
//     p.first=names[idx];
//     while (idx!=0 && popularities[idx-1]==popularities[idx]){
//       idx--;
//     }
//     p.second=1/(double)(idx+1);
//   }
//   return p;
//}
//
//map <int,double> * TopPopContainer::get_recommendation(int num){
//  changed=false;
//  map <int,double> * rec = new map <int,double>;
//  if(num>(int)names.size()) num=names.size();
//  for(int ii=0; ii<num; ii++){
//    rec->insert(make_pair(names[ii],popularities[ii]));
//  }
//  return rec;
//}


void TopPopContainer::swap_up(int item){
  while(idxs[item]!=0 && popularities[idxs[item]]>popularities[idxs[item]-1]){
    int idx=idxs[item];
    swap(idx,idx-1,&names);
    swap(idx,idx-1,&popularities);
    idxs[item]--;
    idxs[names[idx]]++;
  }
}

void TopPopContainer::swap_down(int item){
  while(idxs[item]!=(int)popularities.size()-1 && popularities[idxs[item]+1]>popularities[idxs[item]]){  
    int idx=idxs[item];
    swap(idx,idx+1,&names);
    swap(idx,idx+1,&popularities);
    idxs[item]++;
    idxs[names[idx]]--; 
  }
  squeeze(item);
}

void TopPopContainer::squeeze(int item){
  if(popularities[idxs[item]]==0){
    idxs[item]=-1;
    popularities.pop_back();
    names.pop_back();
  }
}

void TopPopContainer::swap(int idx1, int idx2, vector <int> * vec){
   int buffer=(*vec)[idx1];
   (*vec)[idx1]=(*vec)[idx2];
   (*vec)[idx2]=buffer;
   if(idx1<threshold) changed=true;
}
