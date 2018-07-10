class Model{
  %TypeHeaderCode
  #include "models/Model.h"
  %End
  public:
    Model();
    virtual void add(RecDat * rec_dat);
    virtual double prediction(RecDat * rec_dat) = 0;
    // virtual void write(std::ofstream& file);
    // virtual void read(std::ifstream& file);
    void write(std::string file_name);
    void read(std::string file_name);
    virtual void clear();
    virtual ~Model();
    bool self_test();
};

//this is for SWIG not SIP, but we would have to do something similar:

// %typemap(in) std::ostream& (boost_ofdstream *stream=NULL) {
//   FILE *f=PyFile_AsFile($input); // Verify the semantics of this
//   if (!f) {
//     SWIG_Error(SWIG_TypeError, "File object expected.");
//     SWIG_fail;
//   }
//   else {
//     // If threaded incrment the use count
//     stream = new boost_ofdstream(fileno(f), io::never_close_handle);
//     $1 = new std::ostream(stream);
//   }
// }

// %typemap(in) std::istream& (boost_ifdstream *stream=NULL) {
//   FILE *f=PyFile_AsFile($input); // Verify that this returns NULL for non-files
//   if (!f) {
//     SWIG_Error(SWIG_TypeError, "File object expected.");  
//     SWIG_fail;
//   }
//   else {
//     stream = new boost_ifdstream(fileno(f), io::never_close_handle);
//     $1 = new std::istream(stream);
//   }
// }

// %typemap(freearg) std::ostream& {
//   delete $1;
//   delete stream$argnum;
// }

// %typemap(freearg) std::istream& {
//   delete $1;
//   delete stream$argnum;
// }
