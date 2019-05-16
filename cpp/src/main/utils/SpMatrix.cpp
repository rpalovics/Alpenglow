#include "SpMatrix.h"

void SpMatrix::clear(){
  for(uint ii=0; ii<matrix_.size(); ii++){
    if(matrix_[ii]!=NULL) delete matrix_[ii];
  }
  matrix_.clear();
}

void SpMatrix::read_from_file(string data_file){
  ifstream ifs(data_file.c_str());
  int row_id,col_id;
  double value;
  while(ifs >> row_id >> col_id >> value) insert(row_id,col_id,value);
  cerr << "read from " << data_file << " is done, size: " << size() << endl;
}

void SpMatrix::insert(int row_id, int col_id, double value){
  resize(row_id);
  MatrixRow* row=get_writable(row_id);
  row->insert(make_pair(col_id,value));
}

void SpMatrix::update(int row_id,int col_id, double value){
  resize(row_id);
  MatrixRow* row=get_writable(row_id);
  RowIterator it=row->find(col_id);
  if(it!=row->end()) it->second=value;
  else row->insert(make_pair(col_id,value));
}

void SpMatrix::increase(int row_id,int col_id, double value){
  resize(row_id);
  MatrixRow* row = get_writable(row_id);
  RowIterator it = row->find(col_id);
  if(it!=row->end()) it->second+=value;
  else row->insert(make_pair(col_id,value));
}

void SpMatrix::resize(int row_id){
  if(size()<=row_id) matrix_.resize(row_id+1,NULL);
  if(matrix_[row_id]==NULL){
    matrix_[row_id]= new MatrixRow;
  }
}

int SpMatrix::size() const {
  return (int) matrix_.size();
}

double SpMatrix::get(int row_id, int col_id) const {
  const MatrixRow* row = get(row_id);
  if(row!=NULL){
    auto ri = row->find(col_id);
    if(ri!=row->end()) return ri->second;
    else return 0;
  }
  else return 0;
}


MatrixRow* SpMatrix::get_writable(int row_id) const {
  if(size()>row_id) return matrix_[row_id];
  else return NULL;
}
const MatrixRow* SpMatrix::get(int row_id) const {
  return get_writable(row_id);
}

int SpMatrix::row_size(int row_id) const {
  if(size()>row_id){
    const MatrixRow* row =get(row_id);
    if(row!=NULL) return (int) row->size();
    else return 0;
  }  
  else return 0;
}

void SpMatrix::write_into_file(string file_name) const {
  ofstream ofs(file_name.c_str());
  for(int row_id=0; row_id<size(); row_id++){
    const MatrixRow* row = get(row_id);
    if(row!=NULL){
      for(auto ri = row->begin(); ri!=row->end(); ri++){
        int col_id=ri->first;
        double value=ri->second;
        ofs << row_id << " " << col_id << " " << value << endl;
      }
    }
  }
}

void SpMatrix::erase(int row_id, int col_id){
  MatrixRow* row = get_writable(row_id);
  if(row!=NULL){
    RowIterator ri = row->find(col_id);
    if(ri!=row->end()) row->erase(ri);    
  }
}

bool SpMatrix::has_value(int row_id,int col_id) const {
  const MatrixRow* row =  get(row_id);
  if(row==NULL) return false;
  else if (row->find(col_id)==row->end()) return false;
  else return true;
}

