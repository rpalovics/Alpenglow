import math
import pandas as pd
import numpy as np

def PrecisionScore(test, recommendations, top_k):
    joined = recommendations.join(test.set_index(['user','item']), on=['user','item'], how='inner')
    positives_num = recommendations.groupby('user')['rank'].max()
    hit_num = joined.groupby('user')['item'].count()
    precisions = (hit_num/positives_num).fillna(0)
    return precisions.mean()