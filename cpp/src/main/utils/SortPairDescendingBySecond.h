#ifndef sorter
#define sorter
#include <utility>
using namespace std;

template<typename T>
inline bool sort_pair_descending_by_second(pair<T,double> a, pair<T,double> b){
        return a.second > b.second;
}

template<typename T>
inline bool sort_pair_ascending_by_second(pair<T,double> a, pair<T,double> b){
        return a.second < b.second;
}
#endif
