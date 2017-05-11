from .ParameterSearch import ParameterSearch
import concurrent.futures


class ThreadedParameterSearch(ParameterSearch):
    def __init__(self, model, Score, threads=4, useProcessPool=True):
        ParameterSearch.__init__(self, model, Score)
        self.threads = threads
        self.useProcessPool = useProcessPool

    def run(self, *runParameters, **runKwParameters):
        self.results = []
        configurations = self._getConfigurations()
        parameterTuples = [(parameters, (runParameters, runKwParameters)) for parameters in configurations]

        if(self.useProcessPool):
            executor = concurrent.futures.ProcessPoolExecutor(self.threads)
        else:
            executor = concurrent.futures.ThreadPoolExecutor(self.threads)

        results = list(executor.map(self._mapper, parameterTuples))
        return self._toDataFrame(results)
