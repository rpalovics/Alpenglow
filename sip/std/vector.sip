class VectorInt{
  %TypeHeaderCode
  #include "python/cpp_bridge/VectorType.h"
  %End
public:
  std::vector<int> vec;
  VectorInt(std::vector<int> vec_);
};


template<TYPE>
%MappedType std::vector<TYPE> /TypeHint="list-of-TYPE"/ {
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped P2d.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
       TYPE *cpp = new TYPE(sipCpp -> at(i));
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromNewType(cpp, sipType_TYPE, sipTransferObj)) == NULL) {
           // There was an error so garbage collect the Python list.
           Py_XDECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, pobj);
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode

   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!sipCanConvertToType(item, sipType_TYPE, SIP_NOT_NONE)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of TYPE to  std::vector<TYPE>
   std::vector<TYPE> *v = new std::vector<TYPE>();
   v->reserve(PyList_GET_SIZE(sipPy));
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       int state;
       TYPE* p = reinterpret_cast<TYPE*>(sipConvertToType(PyList_GET_ITEM(sipPy, i), sipType_TYPE, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

     if (*sipIsErr) {
          sipReleaseType(p, sipType_TYPE, state);
          delete v;
          return 0;
       }    

       v->push_back(*p);

       sipReleaseType(p, sipType_TYPE, state);       
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End
};

template<TYPE*>
%MappedType std::vector<TYPE*> /TypeHint="list-of-TYPE"/ {
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped P2d.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromType(sipCpp -> at(i), sipType_TYPE, sipTransferObj)) == NULL) {
           // There was an error so garbage collect the Python list.
           Py_XDECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, pobj);
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode

   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!sipCanConvertToType(item, sipType_TYPE, SIP_NOT_NONE)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of TYPE to std::vector<TYPE*>
   std::vector<TYPE*> *v = new std::vector<TYPE*>();
   v->reserve(PyList_GET_SIZE(sipPy));
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       int state;
       TYPE* p = reinterpret_cast<TYPE*>(sipConvertToType(PyList_GET_ITEM(sipPy, i), sipType_TYPE, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

     if (*sipIsErr) {
          sipReleaseType(p, sipType_TYPE, state);
          delete v;
          return 0;
       }    

       v->push_back(p);

       sipReleaseType(p, sipType_TYPE, state);       
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End
};

// ****************************************************
// Specialization for std::vector<double>
// ****************************************************

%MappedType std::vector<double> /TypeHint="list-of-float"/ {
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyFloat_FromDouble(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyNumber_Check(item)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to double");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of numbers to a std::vector<double>
   std::vector<double> *v = new std::vector<double>();
   v->reserve(PyList_GET_SIZE(sipPy));
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       PyObject *f = PyNumber_Float(PyList_GET_ITEM(sipPy, i));
       v->push_back(PyFloat_AsDouble(f));
       Py_XDECREF(f);       
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End
};


// ****************************************************
// Specialization for std::vector<int>
// ****************************************************

%MappedType std::vector<int> /TypeHint="list-of-int"/ {
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyLong_FromLong(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyLong_Check(item)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to int");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of integers to a std::vector<int>
   std::vector<int> *v = new std::vector<int>();
   v->reserve(PyList_GET_SIZE(sipPy));
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       v->push_back(PyLong_AsLong(PyList_GET_ITEM(sipPy, i)));
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End

};

// ****************************************************
// Specialization for std::vector<unsigned int>
// ****************************************************

%MappedType std::vector<unsigned int> /TypeHint="list-of-int"/ {
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
    // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyLong_FromUnsignedLong(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyLong_Check(item)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to int");
                return 0;
            }
       }
       return 1;
   }

   // Convert Python list of integers to a std::vector<int>
   std::vector<unsigned int> *v = new std::vector<unsigned int>();
   v->reserve(PyList_GET_SIZE(sipPy));
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       v->push_back(PyLong_AsUnsignedLong(PyList_GET_ITEM(sipPy, i)));
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End

};


// ****************************************************
// Specialization for std::vector<bool>
// ****************************************************

%MappedType std::vector<bool> /TypeHint="list-of-bool"/{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyBool_FromLong(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyBool_Check(item)) {
                // printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to bool");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of booleans to a std::vector<bool>
   std::vector<bool> *v = new std::vector<bool>();
   v->reserve(PyList_GET_SIZE(sipPy));  
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       v->push_back(PyList_GET_ITEM(sipPy, i) == Py_True);
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End

};

// ****************************************************
// Specialization for std::vector<bool>
// ****************************************************

%MappedType std::vector<std::pair<int,double>> /TypeHint="list-of-(int, doubl)"/{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   for (int i = 0; i < (int)sipCpp -> size(); ++i) {
      PyObject *pyTuple = PyTuple_New(2);
      PyObject *pyFirst = NULL;
      PyObject *pySecond = NULL;
      pyFirst = PyLong_FromLong(sipCpp->at(i).first);
      pySecond = PyFloat_FromDouble(sipCpp->at(i).second);
      if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
          if (pyFirst)
              Py_XDECREF(pyFirst);
          if (pySecond)
              Py_XDECREF(pySecond);
          Py_XDECREF(pyTuple);
          return 0;
      }
      if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
          Py_XDECREF(pySecond);
          Py_XDECREF(pyTuple);
          return 0;
      }
      PyList_SET_ITEM(l, i, pyTuple);
   }

   return l;
%End

%ConvertToTypeCode
    PyObject *itm1 = NULL;
    PyObject *itm2 = NULL;

   if (sipIsErr == NULL) {
       if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!(PyTuple_Check(item) && PyTuple_Size(item) == 2))
              return 0;

            itm1 = PyTuple_GetItem(item, 0);
            itm2 = PyTuple_GetItem(item, 1);

            bool ok = PyLong_Check(itm1);
            ok = ok && PyFloat_Check(itm2);

            if (!ok) {
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of booleans to a std::vector<bool>
   std::vector<std::pair<int, double>> *v = new std::vector<std::pair<int, double>>();
   v->reserve(PyList_GET_SIZE(sipPy));  
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       PyObject *elem = PyList_GET_ITEM(sipPy, i);
       itm1 = PyTuple_GetItem(elem, 0);
       itm2 = PyTuple_GetItem(elem, 1);

       int ui1 = PyLong_AsLong(itm1);
       double ui2 = PyFloat_AsDouble(itm2);

       std::pair<int, double> p(ui1, ui2);
       v->push_back(p);
   }

   *sipCppPtr = v;
   return sipGetState(sipTransferObj);
%End

};