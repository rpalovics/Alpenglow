#include "FmModelUpdater.h"
#include "../../utils/Util.h"
#include <cmath>

void FmModelUpdater::update(RecDat* rec_dat){
  model_->add(rec_dat);

  const auto &useratt = model_->get_attributes(model_->user_attributes_, rec_dat->user);
  const auto &itematt = model_->get_attributes(model_->item_attributes_, rec_dat->item);
  vector<double> uv = model_->sum_attribute_vector(model_->user_factors_, useratt);
  vector<double> iv = model_->sum_attribute_vector(model_->item_factors_, itematt);
  double prod = Util::scalar_product(&uv, &iv);
  double dif = (rec_dat->score-prod);
  double err = abs(dif);

  model_->update_attributes(model_->user_factors_, useratt, iv, learning_rate_*dif*err);
  model_->update_attributes(model_->item_factors_, itematt, uv, learning_rate_*dif*err);
}
