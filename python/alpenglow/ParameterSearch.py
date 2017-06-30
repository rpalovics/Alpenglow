import itertools
import copy
import pandas as pd


class DependentParameter:
    def __init__(self, formatString, parameterNames=None):
        self.formatString = formatString
        self.parameterNames = parameterNames

    def eval(self, parameters):
        return self.formatString % tuple([parameters[n] for n in self.parameterNames])


class ParameterSearch:
    def __init__(self, model, Score):
        self.model = model
        self.Score = Score
        self.parameterValues = []
        self.parameterNames = []

    def _getNonDependantParameterNames(self):
        return [k for (k, v) in zip(self.parameterNames, self.parameterValues) if not (len(v) == 1 and isinstance(v[0], DependentParameter))]

    def set_parameterValues(self, parameterName, parameterValues):
        if(isinstance(parameterValues, DependentParameter)):
            if(parameterValues.parameterNames is None):
                parameterValues.parameterNames = self._getNonDependantParameterNames()
            parameterValues = [parameterValues]
        self.parameterValues.append(parameterValues)
        self.parameterNames.append(parameterName)

    def _getConfigurations(self):
        names = self.parameterNames
        values = self.parameterValues
        configurations = [dict(zip(names, v)) for v in itertools.product(*values)]
        return configurations

    def _runSingle(self, model, Score, parameters, runParameters):
        model = copy.copy(model)
        for k in parameters:
            model.set_parameter(k, parameters[k])
        scores = model.run(*runParameters[0], **runParameters[1])
        score = Score(scores)
        result = score.mean()
        return result

    def _mapper(self, parameterTuple):
        (parameters, runParameters) = parameterTuple
        currentParameterValues = [parameters[k] for k in self._getNonDependantParameterNames()]
        result = self._runSingle(self.model, self.Score, parameters, runParameters)
        currentParameterValues.append(result)
        return currentParameterValues

    def _toDataFrame(self, results):
        columns = self._getNonDependantParameterNames()
        columns.append(self.Score.__name__)
        return pd.DataFrame.from_records(results, columns=columns)

    def run(self, *runParameters, **runKwParameters):
        self.results = []
        configurations = self._getConfigurations()
        parameterTuples = [(parameters, (runParameters, runKwParameters)) for parameters in configurations]
        results = list(map(self._mapper, parameterTuples))
        return self._toDataFrame(results)
