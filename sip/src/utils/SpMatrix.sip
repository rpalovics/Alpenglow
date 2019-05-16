
//typedef map<int,double> MatrixRow;
//typedef map<int,double>::iterator RowIterator;

class SpMatrix {
  %TypeHeaderCode
  #include "utils/SpMatrix.h"
  %End
  %Docstring
  Implements a sparse matrix. Obtaining a row is O(1), obtaining a value in a
  row is logarithmic in the length of the row.
  %End
  public:
    ~SpMatrix();
    int size() const;
  %Docstring
      Returns
      -------
      The largest row index.
  %End
    void resize(int row_id);
  %Docstring
      resize(int row_id)

      Expands the matrix to contain at least row_id rows. The matrix won't be
      shrunken. Creates an empty row object at index row_id.

      Parameters
      ----------
      row_id : int
        The index of the row.
  %End
    void insert(int row_id, int col_id, double value);
  %Docstring
      insert(int row_id, int col_id, double value)

      Inserts value to (row_id,col_id). If the value already exists, the
      insertion fails silently (the container doesn't change. The matrix is
      expanded and the row is created as necessary.

      Parameters
      ----------
      row_id : int
        The index of the row.
      col_id : int
        The index of the column.
      value : double
        The value to be inserted.
  %End
    void update(int row_id, int col_id, double value);
  %Docstring
      update(int row_id, int col_id, double value)

      Updates (row_id,col_id) to value or inserts value if (row_id,col_id)
      doesn't exist. The matrix is expanded and the row is created as necessary.

      Parameters
      ----------
      row_id : int
        The index of the row.
      col_id : int
        The index of the column.
      value : double
        The new value.
  %End
    void increase(int row_id, int col_id, double value);
  %Docstring
      increase(int row_id, int col_id, double value)

      Increases (row_id,col_id) with value or inserts value if (row_id,col_id)
      doesn't exist (i.e. not existing value is equivalent to 0 value). The
      matrix is expanded and the row is created as necessary.

      Parameters
      ----------
      row_id : int
        The index of the row.
      col_id : int
        The index of the column.
      value : double
        The new value.
  %End
    void erase(int row_id, int col_id);
  %Docstring
      erase(int row_id, int col_id)

      Removes the value from (row_id,col_id). If no value exists at that
      position, nothing happens.

      Parameters
      ----------
      row_id : int
        The index of the row.
      col_id : int
        The index of the column.
  %End
    double get(int row_id, int col_id) const;
  %Docstring
      get(int row_id, int col_id)

      Returns the value of (row_id,col_id). If no value exists at that
      position, returns 0.

      Parameters
      ----------
      row_id : int
        The index of the row.
      col_id : int
        The index of the column.

      Returns
      -------
      double
        The value of (row_id,col_id) or 0 if no value.
  %End
//    const map<int,double>* get(int row_id) const; 

    void clear();
  %Docstring
      Deletes all elements and rows from the matrix.
  %End
    void read_from_file(std::string file_name);
  %Docstring
      read_from_file(std::string file_name)

      Reads matrix from file file_name. Format of the lines of the file is
      "row_id col_id value"
      . In case of repeating row_id col_id pairs, the first value will be used.
      Writes the size of the matrix to stderr. If the file can't be opened,
      fails silently not changing the container.

      Parameters
      ----------
      file_name : std::string
        The name of the file to read from.
  %End
    void write_into_file(std::string file_name) const;
  %Docstring
      write_into_file(std::string file_name)

      Writes matrix into file file_name. Format of the lines of the file is
      "row_id col_id value"
      , space separated. If the file can't be opened, fails silently.

      Parameters
      ----------
      file_name : std::string
        The name of the file to write into.
  %End
    int row_size(int row_id) const;
  %Docstring
      row_size(int row_id)

      Returns the number of the elements of the sparse row row_id. Returns 0 if
      the row doesn't exist.

      Parameters
      ----------
      row_id : int
          The index of the row.

      Returns
      -------
      int
          The size of the row or 0 if the row doesn't exist.
  %End
    bool has_value(int row_id,int col_id) const;
  %Docstring
      has_value(int row_id, int col_id)

      Parameters
      ----------
      row_id : int
          The index of the row.
      col_id : int
          The index of the column.

      Returns
      -------
      bool
          Whether the matrix has value at (row_id,col_id).
  %End
};

