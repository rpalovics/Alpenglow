
#ifndef sorter
#define sorter
#include <utility>
using namespace std;

template<typename T>
inline bool sortPairDescendingBySecond(pair<T,double> a, pair<T,double> b){
        return a.second > b.second;
}

template<typename T>
inline bool sortPairAscendingBySecond(pair<T,double> a, pair<T,double> b){
        return a.second < b.second;
}
#endif
