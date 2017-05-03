import alpenglow.cpp as rs


class AvailabilityFilter(rs.AvailabilityFilter):
    def __init__(self, availabilityData):
        rs.AvailabilityFilter.__init__(self)
        for (time, item, duration) in zip(availabilityData['time'].values, availabilityData['item'].values, availabilityData['duration'].values):
            self.addAvailability(time, item, duration)
