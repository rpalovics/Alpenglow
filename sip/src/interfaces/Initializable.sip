class Initializable {
  %TypeHeaderCode
  #include "interfaces/Initializable.h"
  %End
  %Docstring
    This interface signals that the implementing class has to be initialized by the experiment runner. The experiment runner calls the :py:meth:`initialize()` method, which in return calls the class-specific implementation of :py:meth:`init()` and sets the :py:meth:`is_initialized` flag if the initialization was successful. The :py:meth:`init()` method can check whether the neccessary dependencies have been initialized or not before initializing the instance; and should return the success value accordingly.

      If the initialization was not successful, the experiment runner keeps trying to initialize the not-yet initialized objects, thus resolving dependency chains.
  %End
  public:
    bool is_initialized();
    %Docstring
      Returns
      -------
      bool
          Whether the component has already been initialized.
    %End
    bool initialize();
    %Docstring
      Returns
      -------
      bool
          Whether the initialization was successful.
    %End
    virtual bool init()=0;
    %Docstring
      Has to be implemented by the component.

      Returns
      -------
      bool
          Whether the initialization was successful.
    %End
};