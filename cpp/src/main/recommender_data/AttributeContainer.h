#ifndef INLINE_ATTRIBUTE_READER_H
#define INLINE_ATTRIBUTE_READER_H
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
//templates and inheritance
//template class inherited from template class
class InlineAttributeReader{
  public:
    virtual void read_attribute(int id, string line)=0; //reads attribute that belongs to id
    virtual ~InlineAttributeReader(){}
    bool self_test(){ return true; }
};

template<typename Attribute>
class AttributeContainer : public InlineAttributeReader{ //abstract interface
  public:
    virtual Attribute get_attribute(int id)=0; //returns attribute value belonging to id
    virtual Attribute* get_attribute_ptr(int id)=0; //returns pinter to attribute value belonging to id
    virtual ~AttributeContainer(){}
    bool self_test(){
      bool ok = InlineAttributeReader::self_test();
      return ok;
    }
};

struct GeneralAttributeContainerParameters {
  string filename;
};
template<typename Attribute>
class GeneralAttributeContainer : public AttributeContainer<Attribute>{
  public:
    GeneralAttributeContainer(GeneralAttributeContainerParameters* params){
      filename_ = params->filename;
      if(filename_.length() != 0) read_from_file(); //ha van megadva filenev, beolvassa onnan
    }
    GeneralAttributeContainer(){}
    void read_attribute(int id, string line){ //reads attribute that belongs to id
      if((int)storage_.size()<=id) storage_.resize(id+1);
      Attribute attribute;
      stringstream ss(line);
      ss >> attribute;
      storage_[id] = attribute;
    }
    Attribute get_attribute(int id){ 
      if((int)storage_.size()<=id) return default_attribute;
      else return storage_[id];
    }
    Attribute* get_attribute_ptr(int id){ 
      if((int)storage_.size()<=id) return &default_attribute;
      else return &storage_[id];
    }
    bool self_test(){
      bool ok = AttributeContainer<Attribute>::self_test();
      return ok;
    }
  private:
    vector<Attribute> storage_;
    string filename_;
    Attribute default_attribute;
    void read_from_file(){ //reads attributes from file, format: id attribute, uses operator>> of Attribute
      fstream input_file(filename_.c_str());
      while(!input_file.eof()){
        int id;
        string attribute_string;
        input_file >> id;
        if(input_file.eof()) break;
        input_file.ignore(1, ' ');
        getline(input_file, attribute_string);
        read_attribute(id, attribute_string);
      }
    }
};

#endif /* INLINE_ATTRIBUTE_READER_H */
