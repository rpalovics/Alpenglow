import itertools
import copy
import pandas as pd


class DependentParameter:
    def __init__(self, format_string, parameter_names=None):
        self.format_string = format_string
        self.parameter_names = parameter_names

    def eval(self, parameters):
        return self.format_string % tuple([parameters[n] for n in self.parameter_names])


class ParameterSearch:
    """Utility for evaluating online experiments with different hyperparameters. For a
    brief tutorial on using this class, see :doc:`/getting_started/3-five_minute_tutorial`.
    """
    def __init__(self, model, Score):
        self.model = model
        self.Score = Score
        self.parameter_values = []
        self.parameter_names = []

    def _get_non_dependant_parameter_names(self):
        return [k for (k, v) in zip(self.parameter_names, self.parameter_values) if not (len(v) == 1 and isinstance(v[0], DependentParameter))]

    def set_parameter_values(self, parameter_name, parameter_values):
        if(isinstance(parameter_values, DependentParameter)):
            if(parameter_values.parameter_names is None):
                parameter_values.parameter_names = self._get_non_dependant_parameter_names()
            parameter_values = [parameter_values]
        self.parameter_values.append(parameter_values)
        self.parameter_names.append(parameter_name)

    def _get_configurations(self):
        names = self.parameter_names
        values = self.parameter_values
        configurations = [dict(zip(names, v)) for v in itertools.product(*values)]
        return configurations

    def _run_single(self, model, Score, parameters, run_paremeters):
        model = copy.deepcopy(model)
        for k in parameters:
            model.set_parameter(k, parameters[k])
        scores = model.run(*run_paremeters[0], **run_paremeters[1])
        score = Score(scores)
        result = score.mean()
        return result

    def _mapper(self, parameter_tuple):
        (parameters, run_paremeters) = parameter_tuple
        current_parameter_values = [parameters[k] for k in self._get_non_dependant_parameter_names()]
        result = self._run_single(self.model, self.Score, parameters, run_paremeters)
        current_parameter_values.append(result)
        return current_parameter_values

    def _to_dataframe(self, results):
        columns = self._get_non_dependant_parameter_names()
        columns.append(self.Score.__name__)
        return pd.DataFrame.from_records(results, columns=columns)

    def run(self, *run_paremeters, **run_kw_parameters):
        self.results = []
        configurations = self._get_configurations()
        parameter_tuples = [(parameters, (run_paremeters, run_kw_parameters)) for parameters in configurations]
        results = list(map(self._mapper, parameter_tuples))
        return self._to_dataframe(results)
