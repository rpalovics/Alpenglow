import pandas as pd
import numpy as np
import torch
from recbole.quick_start import run_recbole
from recbole.quick_start import load_data_and_model
from recbole.data.interaction import Interaction
from pathlib import Path

TOPK = 100

def read_users(test_path: str):
    # adjust sep/header if needed; this assumes a column named "user"
    df = pd.read_csv(test_path)
    return df["user"].tolist()

def latest_model_file(checkpoint_dir: str, model_name: str) -> str:
    p = Path(checkpoint_dir)
    candidates = sorted(p.glob(f"{model_name}-*.pth"), key=lambda x: x.stat().st_mtime)
    if not candidates:
        raise FileNotFoundError(f"No {model_name}-*.pth found in {checkpoint_dir}")
    return str(candidates[-1])

parameter_dict = {
    'model': 'BPR',
    'data_path': 'batches',
    'field_separator': ',',
    'USER_ID_FIELD': 'user',
    'ITEM_ID_FIELD': 'item',
    'LABEL_FIELD': 'score',
    'load_col': {
        'inter': ['user','item','score'],
    },

}

for i in range(1,14):
    ckpt_dir = f"batches/batch_{i}"

    #train
    parameter_dict["dataset"] = f"batch_{i}"
    parameter_dict["checkpoint_dir"] = ckpt_dir
    run_recbole(config_dict=parameter_dict)

    #generate top lists
    model_file = latest_model_file(ckpt_dir, "BPR")
    config, model, dataset, *_ = load_data_and_model(model_file=model_file)

    uid_field = config["USER_ID_FIELD"]
    iid_field = config["ITEM_ID_FIELD"]

    # --- users to score (external ids from Alpenglow files) ---
    test_users_ext = read_users(f"{ckpt_dir}/batch_{i}_test.dat")

    # Convert external -> internal (RecBole case study workflow)
    # drop users who didn't appear in the training data,
    # we can't generate a top list for them
    # token2id expects strings in many pipelines; to be safe:
    test_users_int = []
    test_users_ext_filtered = []
    for u_ext in test_users_ext:
        try:
            u_int = dataset.token2id(uid_field, str(u_ext))
            test_users_int.append(u_int)
            test_users_ext_filtered.append(u_ext)
        except ValueError:
            pass #we drop new users
    test_users_ext = test_users_ext_filtered

    if not test_users_int:
        # no eligible users in this batch, no top list file needed
        continue

    # --- full-sort scores ---
    # following recbole.utils.case_study
    interaction = Interaction({uid_field: torch.tensor(test_users_int, dtype=torch.long)}).to(model.device)
    scores = model.full_sort_predict(interaction)  # 1D [n_users * item_num]
    scores = scores.view(len(test_users_int), dataset.item_num)  # reshape

    # mask [PAD] item (internal id 0)
    scores[:, 0] = -np.inf

    # mask interactions already appeared in the training data
    # For each (u,i) seen in training, set score to -inf
    # We only need to mask for the users we are currently scoring.
    user_pos = {u: idx for idx, u in enumerate(test_users_int)}
    training_data = dataset.inter_feat.interaction
    u_int = training_data[uid_field]
    it_int = training_data[iid_field]
    for uu, ii in zip(u_int, it_int):
        row = user_pos.get(uu)
        if row is not None and ii != 0:
            scores[row, ii] = -np.inf

    # --- topK ---
    topk_scores, topk_iids = torch.topk(scores, k=TOPK, dim=1)
    topk_iids = topk_iids.cpu().numpy()

    # Convert internal item ids -> external tokens
    topk_items_ext = dataset.id2token(iid_field, topk_iids.flatten().tolist())
    topk_items_ext = np.array(topk_items_ext).reshape(len(test_users_ext), TOPK)

    # Output format: space separated, no header, columns: user item pos
    out_rows = []
    for uext, items in zip(test_users_ext, topk_items_ext):
        for pos, it in enumerate(items, start=1):
            out_rows.append((uext, it, pos))

    out = pd.DataFrame(out_rows, columns=["user", "item", "pos"])
    out.to_csv(f"batches/batch_{i}_predictions.dat", sep=" ", header=False, index=False)

