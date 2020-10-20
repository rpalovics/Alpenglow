#include "ModelUpdater.h"

void ModelGradientUpdater::message(UpdaterMessage message){
  if(message==UpdaterMessage::start_of_implicit_update_cycle){
    beginning_of_updating_cycle();
  }
  if(message==UpdaterMessage::end_of_implicit_update_cycle){
    end_of_updating_cycle();
  }
}
