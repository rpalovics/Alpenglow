import pandas as pd
import numpy as np
import math

def MseScore(rankings):
    return (rankings['prediction']-rankings['score'])**2
