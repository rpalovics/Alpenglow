import alpenglow.cpp as pr
import threading
from collections import defaultdict

class MetaGetter(type):
    """Metaclass of :py:class:`alpenglow.Getter.Getter`. Provides utilities for
    creating and managing cpp objects in the :py:mod:`alpenglow.cpp` package. For more
    information, see :doc:`/general/memory_management`.
    """
    def __init__(self, a, b, c):
        tid = threading.get_ident()
        self.collect_ = defaultdict(lambda: False)
        self.items = defaultdict(lambda: [])

    def __getattr__(self, name):
        def objectfactory(*args, **kwargs):
            tid = threading.get_ident()
            retval = None
            if(kwargs == {}):
                class_ = getattr(pr, name)
                retval = class_(*args)
            else:
                paramsClass_ = getattr(pr, name + 'Parameters')
                paramval = paramsClass_()
                for k, v in kwargs.items():
                    setattr(paramval, k, v)
                class_ = getattr(pr, name)
                retval = class_(paramval)
            if(self.collect_[tid]):
                self.items[tid].append(retval)
            return retval
        return objectfactory

    def collect(self):
        tid = threading.get_ident()
        self.collect_[tid] = True
        self.items[tid] = []

    def get_and_clean(self):
        tid = threading.get_ident()
        items = self.items[tid]
        self.collect_[tid] = False
        self.items[tid] = []
        return items

    def run_self_test(self, i):
        try:
            if not i.self_test():
                raise AssertionError("Selftest failed for " + i.__repr__())
        except Exception as e:
            if not isinstance(e, AttributeError):
                raise e

    def initialize_all(self, objects):
        objects = [o for o in objects if isinstance(o, pr.Initializable)]
        i = 0
        while i < 10 and objects != []:
            objects = [o for o in objects if not o.initialize()]
            i += 1
        if i == 10 and objects != []:
            raise RuntimeError("Could not initialize objects: " + ",".join([str(o) for o in objects]))

    def set_experiment_environment(self, online_experiment, objects):
        objects = [o for o in objects if isinstance(o, pr.NeedsExperimentEnvironment)]
        for o in objects:
            online_experiment.inject_experiment_environment_into(o)

class Getter(object, metaclass=MetaGetter):
    """Responsible for creating and managing cpp objects in the :py:mod:`alpenglow.cpp`
    package.
    """
    pass
