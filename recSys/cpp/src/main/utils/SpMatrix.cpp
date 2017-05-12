#include "SpMatrix.h"

void SpMatrix::clear(){
  for(uint ii=0; ii<matrix.size(); ii++){
    if(matrix[ii]!=NULL) delete matrix[ii];
  }
  matrix.clear();
}

void SpMatrix::read_from_file(string dataFile){
  ifstream ifs (dataFile.c_str());
  int rowID,colID;
  double value;
  while(ifs >> rowID >> colID >> value) insert(rowID,colID,value);
  cerr << "read from " << dataFile << " is done, size: " << size() << endl;
}

void SpMatrix::insert(int rowID, int colID, double value){
  resize(rowID);
  MatrixRow * row=get(rowID);
  row->insert(make_pair(colID,value));
}

void SpMatrix::update(int rowID,int colID, double value){
  resize(rowID);
  MatrixRow * row=get(rowID);
  MatrixRow :: iterator it=row->find(colID);
  if(it!=row->end()) it->second=value;
  else row->insert(make_pair(colID,value));
}

void SpMatrix::increase(int rowID,int colID, double value){
  resize(rowID);
  MatrixRow * row = get(rowID);
  MatrixRow :: iterator it = row->find(colID);
  if(it!=row->end()) it->second+=value;
  else row->insert(make_pair(colID,value));
}

void SpMatrix::resize(int rowID){
  if(size()<=rowID) matrix.resize(rowID+1,NULL);
  if(matrix[rowID]==NULL){
    matrix[rowID]= new MatrixRow;
  }
}

int SpMatrix::size(){
  return (int) matrix.size();
}

double SpMatrix::get(int rowID, int colID){
  MatrixRow * row = get(rowID);
  if(row!=NULL){
    RowIterator ri = row->find(colID);
    if(ri!=row->end()) return ri->second;
    else return 0;
  }
  else return 0;
}


MatrixRow * SpMatrix::get(int rowID){
  if(size()>rowID) return matrix[rowID];
  else return NULL;
}

int SpMatrix::row_size(int rowID){
  if(size()>rowID){
    MatrixRow * row =get(rowID);
    if(row!=NULL) return (int) row->size();
    else return 0;
  }  
  else return 0;
}

void SpMatrix::write_into_file(string fileName){
  ofstream ofs(fileName.c_str());
  for(int rowID=0; rowID<size(); rowID++){
    MatrixRow * row = get(rowID);
    if(row!=NULL){
      for(RowIterator ri = row->begin(); ri!=row->end(); ri++){
        int colID=ri->first;
        double value=ri->second;
        ofs << rowID << " " << colID << " " << value << endl;
      }
    }
  }
}

void SpMatrix::erase(int rowID, int colID){
  MatrixRow * row = get(rowID);
  if(row!=NULL){
    RowIterator ri = row->find(colID);
    if(ri!=row->end()) row->erase(ri);    
  }
}

bool SpMatrix::has_value(int rowID,int colID){
  MatrixRow * row =  get(rowID);
  if(row==NULL) return false;
  else if (row->find(colID)==row->end()) return false;
  else return true;
}

