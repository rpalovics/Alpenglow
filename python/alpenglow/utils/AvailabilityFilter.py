import alpenglow.cpp as rs


class AvailabilityFilter(rs.AvailabilityFilter):
    """Python wrapper around :py:class:`alpenglow.cpp.AvailabilityFilter`.
    """
    def __init__(self, availability_data):
        rs.AvailabilityFilter.__init__(self)
        for (time, item, duration) in zip(availability_data['time'].values, availability_data['item'].values, availability_data['duration'].values):
            self.add_availability(time, item, duration)
