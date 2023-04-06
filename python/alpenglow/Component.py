from .Getter import Getter
from .ParameterDefaults import ParameterDefaults

class Component():
    def __init__(self, **parameters):
        self.parameter_container = ParameterDefaults(**parameters)
        self._catalog = self._get_catalog()
        self._built = {}

    def show(self):
        """
        Displays nodes and connections.
        """
        print(self._catalog)
        #TODO print in a ascii graph form, like Graph::Easy in Perl

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
          if description["object"]==None:
            object_type=description["type"]
            object_class=getattr(Getter,object_type)
            default_parameters=description["parameters"]
            parameters=self.parameter_container.parameter_defaults(**default_parameters)
            description["object"]=object_class(**parameters)

        for name,description in self._catalog.items():
          instance=description["object"]
          for function_name,parameter in description["connections"]:
            function=getattr(instance, function_name)
            parameter_instance=self._catalog[parameter]["object"]
            function(parameter_instance)

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

    def _get_catalog(self):
        """Needs to be implemented by subclasses. Returns the dictionary describing the object structure of this component."""
        pass

