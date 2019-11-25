import pandas as pd
from alpenglow.offline.models import FactorModel
import alpenglow.Getter as rs

data = pd.read_csv(
    "../../python/test_alpenglow/test_data_4",
    sep=' ',
    header=None,
    names=['time', 'user', 'item', 'id', 'score', 'eval']
)
model = FactorModel(
    factor_seed=254938879,
    dimension=10,
    negative_rate=9,
    number_of_iterations=20,
)
model.fit(data)

model.model.write("output_file") #writes model to output_file
rd = rs.RecDat()
rd.user = 3
rd.item = 5
print("prediction for user=3, item=5:", model.model.prediction(rd))

#model2 must have the same dimension
model2 = FactorModel(
    factor_seed=1234,
    dimension=10,
    negative_rate=0,
    number_of_iterations=0,
)
#to create the inner model but avoid training, we need to run fit()
#on an empty dataset
data2=pd.DataFrame(columns=['time', 'user', 'item'])
model2.fit(data2)
model2.model.read("output_file") #reads back the same model
print("prediction for user=3, item=5 using the read-back model:", model2.model.prediction(rd))
