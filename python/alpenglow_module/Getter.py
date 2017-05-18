import alpenglow.cpp as pr


class MetaGetter(type):
    def __init__(self, a, b, c):
        self.collect_ = False
        self.items = []

    def __getattr__(self, name):
        def objectfactory(*args, **kwargs):
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
            if(self.collect_):
                self.items.append(retval)
            return retval
        return objectfactory

    def collect(self):
        self.collect_ = True
        self.items = []

    def get_and_clean(self):
        items = self.items
        self.collect_ = False
        self.items = []
        return items

    def run_self_test(self, i):
        try:
            if not i.self_test():
                raise AssertionError("Selftest failed for " + i.__repr__())
        except Exception as e:
            if not isinstance(e, AttributeError):
                raise e


class Getter(object, metaclass=MetaGetter):
    pass
