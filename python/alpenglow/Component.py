from .Getter import Getter
from .ParameterDefaults import ParameterDefaults

class Component():
    def __init__(self, **parameters):
        if "parameter_container" in parameters.keys() :
          self.parameter_container = parameters["parameter_container"]
        else :
          self.parameter_container = ParameterDefaults(**parameters)
        self._catalog = self._get_catalog()
        self._built = False

    def get_parameter_container(self):
        return self.parameter_container

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
        if self._built :
          raise RuntimeError("Replacing objects is not possible after building the component.")
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        canonical_name = self.resolve_alias(name)
        self._catalog[canonical_name]["object"] = obj

    def build(self):
        """
        Creates the missing objects from the catalog and wires them.
        """
        #TODO renamed parameters
        Getter.collect()
        for name,description in self._catalog.items():
          if self.is_alias(name) :
            continue
          if description["object"]==None:
            object_type=description["type"]
            object_class=getattr(Getter,object_type)
            default_parameters=description["parameters"]
            parameters=self.parameter_container.parameter_defaults(**default_parameters)
            description["object"]=object_class(**parameters)

        for name,description in self._catalog.items():
          if self.is_alias(name) :
            continue
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
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        canonical_name = self.resolve_alias(name)
        obj = self._catalog[canonical_name]["object"]
        if obj == None :
          raise RuntimeError("This object does not exist yet. Run build() to create all objects.")
        return obj

    def resolve_alias(self, name):
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        description = self._catalog[name]
        if "alias_for" in description.keys() :
          name = description["alias_for"]
        return name

    def is_alias(self,name):
        if name not in self._catalog:
          raise RuntimeError("No object named "+name+" in this component.")

        description = self._catalog[name]
        return "alias_for" in description.keys()

    def _get_catalog(self):
        """Needs to be implemented by subclasses. Returns the dictionary describing the object structure of this component."""
        pass

