from alpenglow.cpp import PythonModel, PythonToplistModel, Updater, PythonRankingIteratorModel



class SubModel(PythonModel):
    def __init__(self, parent):
        super(SubModel, self).__init__()
        self.parent = parent

    def prediction(self, rec_dat):
        return self.parent.prediction(rec_dat)

class SubToplistModel(PythonToplistModel):
    def __init__(self, parent):
        super(SubToplistModel, self).__init__()
        self.parent = parent

    def prediction(self, rec_dat):
        return self.parent.prediction(rec_dat)

    def get_top_list(self, u, k, exclude):
        return self.parent.get_top_list(u, k, exclude)


class SubRankingIteratorModel(PythonRankingIteratorModel):
    def __init__(self, parent):
        super(SubRankingIteratorModel, self).__init__()
        self.parent = parent

        self.bound_getter = lambda: self.bound
    
        self.actual_id = None
        self.bound = None
        self.actual_iterator = None
        self.next_value = None

    def prediction(self, rec_dat):
        return self.parent.prediction(rec_dat)

    def iterator_has_next_(self, id_, user, upper_bound):
        self.bound = upper_bound
        if id_ != self.actual_id:
            self.actual_iterator = self.parent.prediction_iterator(user, self.bound_getter)
            self.actual_id = id_
            self.next_value = None
        try:
            (item, score) = next(self.actual_iterator)
            self.next_value = (int(item), float(score))
            return True
        except StopIteration:
            return False

    def iterator_get_next_(self, id, user):
        return self.next_value

class SubUpdater(Updater):
    def __init__(self, parent):
        super(SubUpdater, self).__init__()
        self.parent = parent

    def update(self, rec_dat):
        return self.parent.update(rec_dat)

class SelfUpdatingModel:
    def __init__(self):
        self._updater = SubUpdater(self)
        if hasattr(self, 'get_top_list') and callable(getattr(self, 'get_top_list', None)):
            self._model = SubToplistModel(self)
        if hasattr(self, 'prediction_iterator') and callable(getattr(self, 'prediction_iterator', None)):
            self._model = SubRankingIteratorModel(self)
        else:
            self._model = SubModel(self)
        super(SelfUpdatingModel, self).__init__()
        