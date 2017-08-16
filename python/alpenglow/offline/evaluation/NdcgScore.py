import math
import pandas as pd
import numpy as np

def NdcgScore(test, recommendations, top_k=100):
    to_join=test\
        .copy()\
        .sort_values('score',ascending=False)\
        .drop_duplicates(subset=['user','item'])[['user','item','score']]\
        .rename(columns={'score':'score_test'})\
        .set_index(['user','item'])
    recs = recommendations.join(to_join, on=['user','item'], how='inner').copy()
    users = set(recommendations['user']) & set(test['user'])
    idcgs = pd.DataFrame({'user':list(users)}).set_index('user').sort_index()
    ranked_test=test.sort_values('score',ascending=False).copy()
    ranked_test['rank']=1
    ranked_test['rank']=ranked_test.groupby('user')['rank'].transform(pd.Series.cumsum)
        
    ranked_test = ranked_test[ranked_test['rank']<=100].copy()
    ranked_test['dg']=ranked_test['score']/np.log2(ranked_test['rank']+1)
    idcgs['idcg'] = ranked_test.groupby('user')['dg'].sum()
    recs['dg']=recs['score_test']/np.log2(recs['rank']+1)
    dcgs = recs.groupby('user')['dg'].sum()
    ndcgs = (dcgs/idcgs['idcg']).fillna(0)
    ndcg = ndcgs.mean()

    return ndcg