
class PopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  %Docstring
    Container for storing the popularity of items.
  %End
  public:
    void increase(int item);
  %Docstring
      increase(int item)
      Increases the popularity of the item.

      Parameters
      ----------
      item : int
          The item.
  %End
    void reduce(int item);
  %Docstring
      reduce(int item)
      Reduces the popularity of the item.

      Parameters
      ----------
      item : int
          The item.
  %End
    int get(int item) const;
  %Docstring
      get(int item)
      Returns the popularity value of the item.

      Parameters
      ----------
      item : int
          The item.

      Returns
      -------
      int
          The popularity value of the item.
  %End
    void clear();
  %Docstring
      Clears the container. After this operations, the popularity of all the items is 0.
  %End
};

class TopPopContainer{
  %TypeHeaderCode
  #include "utils/PopContainers.h"
  %End
  %Docstring
    Helper class for storing the items sorted by popularity.

    Sample code

    .. code-block:: python
      :linenos:

      x = rs.TopPopContainer()
      x.increase(1)
      x.increase(1)
      x.increase(3)
      x.increase(4)
      x.increase(1)

      print("The most popular item is")
      print(x.get_item(0)) #returns 1
      print("The second most popular item is")
      print(x.get_item(1)) #returns 3 or 4

  %End
  public:
    void increase(int item);
  %Docstring
      increase(int item)
      Increases the popularity of the item.
  %End
    void create(int item);
  %Docstring
      create(int item)
      Adds an item to the container. The item will have 0 popularity, but will be counted in :py:meth:`size`.
  %End
//    pair<int,double> get(int idx) const;

    int get_item(int idx) const;
  %Docstring
      get_item(int index)
      Returns the index'th item from the popularity toplist.

      Parameters
      ----------
      index : int
          Index of the item in the popularity toplist. The index of the most popular item is 0.

      Returns
      -------
      int
          The appropriate item from the toplist.
  %End
    int size() const;
  %Docstring
      Returns the length of the complete toplist, i.e. the number of items having at least a popularity of 1 or that were added through :py:meth:`create`.

      Returns
      -------
      int
          The length of the complete toplist.
  %End
};

