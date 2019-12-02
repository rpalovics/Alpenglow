template<T1, T2>
%MappedType std::map<T1, T2> /TypeHint="dict-of-T1-T2"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
  
  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;

  if (sipIsErr == NULL) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = sipCanConvertToType(key, sipType_T1, SIP_NOT_NONE);
      ok = ok && sipCanConvertToType(val, sipType_T2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }

  
  std::map <T1, T2> *map = new std::map<T1, T2>();
  
  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t1State = 0;
    int t2State = 0;
    
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, sipType_T1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, sipType_T2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;   
      sipReleaseType(t1, sipType_T1, t1State);
      sipReleaseType(t2, sipType_T2, t2State);
      return 0;
    }
    
    (*map)[*t1] = *t2;
    
    sipReleaseType(t1, sipType_T1, t1State);
    sipReleaseType(t2, sipType_T2, t2State);
  }
  
  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
  PyObject *pyDict = NULL;
  pyDict = PyDict_New();
  if (pyDict == NULL) {
    return NULL;
  }
  
  for (std::map<T1, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = NULL;
    PyObject *val = NULL;
    T1 *cppKey = new T1(it->first);
    T2 *cppVal = new T2(it->second);
    
    key = sipConvertFromNewType(cppKey, sipType_T1, sipTransferObj);
    val = sipConvertFromNewType(cppVal, sipType_T2, sipTransferObj);
    
    if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);
      
      if (key) {
        Py_DECREF(key);
      } else {
        delete cppKey;
      }
      
      if (val != NULL) {
        Py_DECREF(val);
      } else {
        delete cppVal;
      }
      
      return NULL;
    }
    
    Py_DECREF(key);
    Py_DECREF(val);
  }
  
  return pyDict;
%End
};

template<T1, T2>
%MappedType std::map<T1, T2 *> /TypeHint="dict-of-T1-T2"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;

  if (sipIsErr == NULL) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = sipCanConvertToType(key, sipType_T1, SIP_NOT_NONE);
      ok = ok && sipCanConvertToType(val, sipType_T2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }
  
  std::map <T1, T2 *> *map = new std::map<T1, T2 *>();
  
  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t1State = 0;
    int t2State = 0;
    
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, sipType_T1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, sipType_T2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;
      sipReleaseType(t1, sipType_T1, t1State);
      sipReleaseType(t2, sipType_T2, t2State);
      return 0;
    }
    
    (*map)[*t1] = t2;
    
    sipReleaseType(t1, sipType_T1, t1State);
    sipReleaseType(t2, sipType_T2, t2State);
  }
  
  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
  PyObject *pyDict = NULL;
  pyDict = PyDict_New();
  if (pyDict == NULL) {
    return NULL;
  }
  
  for (std::map<T1, T2 *>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = NULL;
    PyObject *val = NULL;
    T1 *cppKey = new T1(it->first);
    T2 *cppVal = it->second;
    
    key = sipConvertFromNewType(cppKey, sipType_T1, sipTransferObj);
    val = sipConvertFromType(cppVal, sipType_T2, sipTransferObj);
    
    if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);
      
      if (key) {
        Py_DECREF(key);
      } else {
        delete cppKey;
      }
      
      if (val != NULL) {
        Py_DECREF(val);
      }
      
      return NULL;
    }
    
    Py_DECREF(key);
    Py_DECREF(val);
  }
  
  return pyDict;
%End
};

template<T2>
%MappedType std::map<float, T2> /TypeHint="dict-of-float-T2"/{
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
  
  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;

  if (sipIsErr == NULL) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = PyFloat_Check(key);
      ok = ok && sipCanConvertToType(val, sipType_T2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }

  
  std::map <float, T2> *map = new std::map<float, T2>();
  
  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t2State = 0;
    
    float t1 = PyFloat_AsDouble(key);
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, sipType_T2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;   
      sipReleaseType(t2, sipType_T2, t2State);
      return 0;
    }
    
    (*map)[t1] = *t2;
    
    sipReleaseType(t2, sipType_T2, t2State);
  }
  
  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
  PyObject *pyDict = NULL;
  pyDict = PyDict_New();
  if (pyDict == NULL) {
    return NULL;
  }
  
  for (std::map<float, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = NULL;
    PyObject *val = NULL;
    float cppKey = it->first;
    T2 *cppVal = new T2(it->second);
    
    key = PyFloat_FromDouble(cppKey);
    val = sipConvertFromNewType(cppVal, sipType_T2, sipTransferObj);
    
    if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);
      
      if (key) {
        Py_DECREF(key);
      } 
      
      if (val != NULL) {
        Py_DECREF(val);
      } else {
        delete cppVal;
      }
      
      return NULL;
    }
    
    Py_DECREF(key);
    Py_DECREF(val);
  }
  
  return pyDict;
%End
};


%MappedType std::map<int, double> /TypeHint="dict-of-int-double"/{
  %TypeHeaderCode
    #include <map>
  %End

  %ConvertToTypeCode
    PyObject *key = NULL;
    PyObject *val = NULL;
    Py_ssize_t pos = 0;

    if (sipIsErr == NULL) {
      if (!PyDict_Check(sipPy)) {
        return 0;
      }
      while (PyDict_Next(sipPy, &pos, &key, &val)) {
        bool ok = PyLong_Check(key);
        ok = ok && PyFloat_Check(val);
        if (!ok) {
          return 0;
        }
      }
      return 1;
    }

    std::map <int, double> *map = new std::map<int, double>();
    
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      int t2State = 0;
      
      int t1 = PyLong_AsLong(key);
      double t2 = PyFloat_AsDouble(val);
  
      if (*sipIsErr) {
        delete map;   
        return 0;
      }
      
      (*map)[t1] = t2;
    }
    
    *sipCppPtr = map;
    return sipGetState(sipTransferObj);
  %End

  %ConvertFromTypeCode
    PyObject *pyDict = NULL;
    pyDict = PyDict_New();
    if (pyDict == NULL) {
      return NULL;
    }
    
    for (std::map<int, double>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
      PyObject *key = NULL;
      PyObject *val = NULL;
      int cppKey = it->first;
      double cppVal = it->second;
      
      key = PyLong_FromLong(cppKey);
      val = PyFloat_FromDouble(cppVal);
      
      if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
        Py_DECREF(pyDict);
        
        if (key) {
          Py_DECREF(key);
        } 
        
        if (val != NULL) {
          Py_DECREF(val);
        }
        
        return NULL;
      }
      
      Py_DECREF(key);
      Py_DECREF(val);
    }
    
    return pyDict;
  %End
};