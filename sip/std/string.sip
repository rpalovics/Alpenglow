// Creates the mapping for std::string
// From: http://www.riverbankcomputing.com/pipermail/pyqt/2009-July/023533.html
// for py3: https://github.com/orocos/orocos_kinematics_dynamics/blob/33400d4adafe91793ae91055026366b2d1dd76e5/python_orocos_kdl/PyKDL/std_string.sip

%MappedType std::string
{
%TypeHeaderCode
#include <string>
%End

%ConvertFromTypeCode
    // convert an std::string to a Python (unicode) string
    PyObject* newstring;
    newstring = PyUnicode_DecodeUTF8(sipCpp->c_str(), sipCpp->length(), NULL);
    if(newstring == NULL) {
        PyErr_Clear();
        newstring = PyUnicode_FromString(sipCpp->c_str());
    }
    return newstring;
%End

%ConvertToTypeCode
     // Allow a Python string (or a unicode string) whenever a string is
     // expected.
    // If argument is a Unicode string, just decode it to UTF-8
    // If argument is a Python string, assume it's UTF-8
     if (sipIsErr == NULL)
#if PY_MAJOR_VERSION < 3
        return (PyString_Check(sipPy) || PyUnicode_Check(sipPy));
#else
        return PyUnicode_Check(sipPy);
#endif
     if (sipPy == Py_None) {
        *sipCppPtr = new std::string;
         return 1;
     }
     if (PyUnicode_Check(sipPy)) {
        *sipCppPtr = new std::string(PyUnicode_AsUTF8(sipPy));
        return 1;
     }
#if PY_MAJOR_VERSION < 3
     if (PyString_Check(sipPy)) {
        *sipCppPtr = new std::string(PyString_AS_STRING(sipPy));
        return 1;
     }
#endif

     return 0;
%End
};