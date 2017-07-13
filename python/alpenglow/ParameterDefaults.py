from .ParameterSearch import DependentParameter

class ParameterDefaults:
    def __init__(self, **parameters):
        self.parameters = parameters
        self.given_parameters = set(self.parameters.keys())
        self.used_parameters = set([])

    def check_unused_parameters(self):
        unused = self.given_parameters - self.used_parameters
        if(len(unused) != 0):
            raise TypeError("Unused parameters: " + ", ".join(unused))

    def set_parameter(self, name, value):
        self.parameters[name] = value
        self.given_parameters |= set([name])

    def parameter_defaults(self, **defaults):
        for k in defaults:
            defaults[k] = self.parameter_default(k, defaults[k])
        return defaults

    def parameter_default(self, name, value):
        self.used_parameters |= set([name])
        if name in self.parameters:
            if isinstance(self.parameters[name], DependentParameter):
                return self.parameters[name].eval(self.parameters)
            else:
                return self.parameters[name]
        else:
            return value
