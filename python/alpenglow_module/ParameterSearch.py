import itertools
import copy
import pandas as pd


class ParameterSearch:
    def __init__(self, model, Score):
        self.model = model
        self.Score = Score
        self.parameterValues = []
        self.parameterNames = []

    def setParameterValues(self, parameterName, parameterValues):
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
            model.setParameter(k, parameters[k])
        scores = model.run(*runParameters[0], **runParameters[1])
        score = Score(scores)
        result = score.evaluate()
        return result

    def _mapper(self, parameterTuple):
        (parameters, runParameters) = parameterTuple
        currentParameterValues = [parameters[k] for k in self.parameterNames]
        result = self._runSingle(self.model, self.Score, parameters, runParameters)
        return (*currentParameterValues, result)

    def _toDataFrame(self, results):
        return pd.DataFrame.from_records(results, columns=(*self.parameterNames, self.Score.__name__))

    def run(self, *runParameters, **runKwParameters):
        self.results = []
        configurations = self._getConfigurations()
        parameterTuples = [(parameters, (runParameters, runKwParameters)) for parameters in configurations]
        results = list(map(self._mapper, parameterTuples))
        return self._toDataFrame(results)
