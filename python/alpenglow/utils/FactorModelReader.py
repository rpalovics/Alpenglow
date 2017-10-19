import pandas as pd


def readFactorModel(file, dimensions):
    import alpenglow.Getter as ag
    """Utility for reading binary models --saved by online experiments-- into
    pandas DataFrames.
    """
    r = ag.FactorModelReader()
    uif = r.read(file, dimensions)

    users = []
    user_factors = []
    for f in uif.user_factors:
        users.append(f.entity)
        user_factors.append(f.factors)
    items = []
    item_factors = []
    for f in uif.item_factors:
        items.append(f.entity)
        item_factors.append(f.factors)

    user_df = pd.DataFrame.from_records(user_factors, columns=range(1, dimensions + 1))
    user_df['user'] = users
    user_df.set_index('user', inplace=True)

    item_df = pd.DataFrame.from_records(item_factors, columns=range(1, dimensions + 1))
    item_df['item'] = items
    item_df.set_index('item', inplace=True)

    return (user_df, item_df)

def readEigenFactorModel(file):
    import alpenglow.Getter as ag
    """Utility for reading binary models --saved by online experiments-- into
    pandas DataFrames.
    """
    r = ag.EigenFactorModelReader()
    uif = r.read(file)

    users = []
    user_factors = []
    for f in uif.user_factors:
        users.append(f.entity)
        user_factors.append(f.factors)
    items = []
    item_factors = []
    for f in uif.item_factors:
        items.append(f.entity)
        item_factors.append(f.factors)

    user_df = pd.DataFrame.from_records(user_factors, columns=range(1, len(user_factors[0])+1))
    user_df['user'] = users
    user_df.set_index('user', inplace=True)

    item_df = pd.DataFrame.from_records(item_factors, columns=range(1, len(item_factors[0])+1))
    item_df['item'] = items
    item_df.set_index('item', inplace=True)

    return (user_df, item_df)
