from .Getter import Getter
from .ParameterDefaults import ParameterDefaults

class Component(ParameterDefaults):
    def __init__(self, **parameters):
        super().__init__(**parameters)
        self._catalog = self._get_catalog()
        self._built = {}

    def show(self):
        """
        Lists nodes and connections.
        """
        self.print_catalog()
        #TODO

    def set_object(self, name, obj):
        """
        Replaces a default object in the catalog to an already built one
        provided as a parameter.
        """
        #TODO aliases
        if self._built :
          raise RuntimeError("Replacing objects is not possible after building the component.")
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        self._catalog[name]["object"] = obj

    def build(self):
        """
        Creates the missing objects from the catalog and wires them.
        """
        #TODO aliases, renamed parameters
        for name,description in self._catalog.items():
          object_type=description["type"]
          object_class=getattr(Getter,object_type)
          my_object=object_class(**self.parameter_defaults(**description["parameters"]))
          description["object"]=my_object

        for name,description in self._catalog.items():
          my_object=description["object"]
          for function_name,parameter in description["connections"]:
            function=getattr(my_object, function_name)
            parameter_object=self._catalog[parameter]["object"]
            function(parameter_object)

        self._built = True

    def get_object(self, name):
        """
        Returns the object named name.
        """
        #TODO aliases
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        obj = self._catalog[name]["object"]
        if obj == None :
          raise RuntimeError("This object does not exist yet. Run build() to create all objects.")
        return obj

    def print_catalog(self):
        """Prints the catalog for debug purposes. Will be removed later."""
        print(self._catalog)

    def _get_catalog(self):
        """Returns the dictionary describing the object structure of this component."""
        return {}

