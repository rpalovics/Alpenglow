import math
import pandas as pd
import numpy as np

def RecallScore(test, recommendations, top_k):
    joined = recommendations.join(test.set_index(['user','item']), on=['user','item'], how='inner')
    hit_num = joined.groupby('user')['item'].count()
    positives_num = test.groupby('user')['item'].nunique()
    recalls = (hit_num/positives_num).fillna(0)

    return recalls.mean()