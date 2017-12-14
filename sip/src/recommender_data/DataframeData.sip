class DataframeData : public RecommenderData {
  %TypeHeaderCode
  #include "python/cpp_bridge/DataframeData.h"
  #include <numpy/arrayobject.h>
  #define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
  %End
  public:
  	void add_recdats(
  		SIP_PYOBJECT* users /In/,
  		SIP_PYOBJECT* items /In/,
  		SIP_PYOBJECT* ids /In/,
  		SIP_PYOBJECT* scores /In/,
  		SIP_PYOBJECT* times /In/,
  		SIP_PYOBJECT* evals /In/,
  		SIP_PYOBJECT* categories /In/
  	);
	%MethodCode
		Py_BEGIN_ALLOW_THREADS //for sip 3.x compatibility this is required
		SIP_BLOCK_THREADS //however numpy header access requres blocking threads

		//initialize numpy array api if not already initialized
		if(PyArray_API == NULL){
	    	import_array();
		}

		//get direct access to users array data
		long *users_ptr = NULL;
		PyObject *users = PyArray_FROM_OTF(a0, NPY_LONG, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(users == NULL){
			Py_XDECREF(users);
		} else {
			users_ptr = (long*)PyArray_DATA(users);
		}
		
		long *items_ptr = NULL;
		PyObject *items = PyArray_FROM_OTF(a1, NPY_LONG, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(items == NULL){
			Py_XDECREF(items);
		} else {
			items_ptr = (long*)PyArray_DATA(items);
		}
		
		long *ids_ptr = NULL;
		PyObject *ids = PyArray_FROM_OTF(a2, NPY_LONG, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(ids == NULL){
			Py_XDECREF(ids);
		} else {
			ids_ptr = (long*)PyArray_DATA(ids);
		}
		
		double *scores_ptr = NULL;
		PyObject *scores = PyArray_FROM_OTF(a3, NPY_DOUBLE, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(scores == NULL){
			Py_XDECREF(scores);
		} else {
			scores_ptr = (double*)PyArray_DATA(scores);
		}
		
		double *times_ptr = NULL;
		PyObject *times = PyArray_FROM_OTF(a4, NPY_DOUBLE, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(times == NULL){
			Py_XDECREF(times);
		} else {
			times_ptr = (double*)PyArray_DATA(times);
		}
		
		long *evals_ptr = NULL;
		PyObject *evals = PyArray_FROM_OTF(a5, NPY_LONG, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(evals == NULL){
			Py_XDECREF(evals);
		} else {
			evals_ptr = (long*)PyArray_DATA(evals);
		}
		
		long *categories_ptr = NULL;
		PyObject *categories = PyArray_FROM_OTF(a6, NPY_LONG, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ENSUREARRAY);
		if(categories == NULL){
			Py_XDECREF(categories);
		} else {
			categories_ptr = (long*)PyArray_DATA(categories);
		}
		
		// Call the function
		if(users_ptr != NULL 
			&& items_ptr != NULL 
			&& ids_ptr != NULL 
			&& scores_ptr != NULL 
			&& times_ptr != NULL
			&& evals_ptr != NULL
			&& categories_ptr != NULL
		){
			sipCpp->add_recdats(PyArray_SIZE(users), users_ptr, items_ptr, ids_ptr, scores_ptr, times_ptr, evals_ptr, categories_ptr);
		}

	    SIP_UNBLOCK_THREADS
		Py_END_ALLOW_THREADS
	%End
    int size();
    RecDat* get(int idx);
};
