// typedef map <int,double> MatrixRow;
// typedef map <int,double> :: iterator RowIterator;

class SpMatrix{
  %TypeHeaderCode
  #include "utils/SpMatrix.h"
  %End
  public:
    SpMatrix();
    ~SpMatrix();
    void readFromFile(std::string file_name);
    void writeIntoFile(std::string file_name);
    int size();
    int rowSize(int rowID);
    void resize(int rowID);
    void insert(int rowID, int colID, double value);
    void update(int rowID, int colID , double value);
    void increase(int rowID, int colID, double value);
    void erase(int rowID, int colID);
    double get(int rowID, int colID);
    // std::map<int,double> * get(int rowID);
    bool hasValue(int rowID,int colID);
    void clear();
};