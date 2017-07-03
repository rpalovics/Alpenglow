from .ParameterSearch import ParameterSearch
import concurrent.futures


class ThreadedParameterSearch(ParameterSearch):
    def __init__(self, model, Score, threads=4, use_process_pool=True):
        ParameterSearch.__init__(self, model, Score)
        self.threads = threads
        self.use_process_pool = use_process_pool

    def run(self, *run_parameters, **run_kw_parameters):
        self.results = []
        configurations = self._get_configurations()
        parameter_tuples = [(parameters, (run_parameters, run_kw_parameters)) for parameters in configurations]

        if(self.use_process_pool):
            executor = concurrent.futures.ProcessPoolExecutor(self.threads)
        else:
            executor = concurrent.futures.ThreadPoolExecutor(self.threads)

        results = list(executor.map(self._mapper, parameter_tuples))
        return self._to_dataframe(results)
