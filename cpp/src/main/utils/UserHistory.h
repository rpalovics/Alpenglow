#ifndef USER_HISTORY
#define USER_HISTORY
#include<exception>

#include "../recommender_data/RecommenderData.h"
#include "../models/ModelUpdater.h"

//kerdes: ha egy user ugyanazt tobbszor hallgatja, akkor a torteneteben is tobbszor szerepel
//szamitasigenyes megoldani, hogy az idoben renezett listaban csak az idoben legfrissebb minta szerepeljen
//ha matrixban taroljuk, ugy konnyu, de nem lesz idoben rendezett
//lehet SimpleUpdater leszarmazott, ekkor be lehet settelni ot magat a learnerbe
//session-kezeles: be lehet irni az updaterbe, vagy lehet kivulrol hivogatni a delete fuggvenyeket
//policy lesz, ha model_simple_updater lesz: ha valahova be van settelve (jinjactor), akkor ot is mint simpleupdatert be kell settelni. Ha lokalisan hozodik letre, akkor a szulo gondoskodik a frissitesrol.
//most azert nem az, mert a modelsimpleupdater nem const recdatot var parameterul, pedig azt is varhatna
//altalanositasi lehetoseg: type parameter, ami lehet user, item, label, legutobbi esetben labelcontainer alapjan megy az update; user pedig mindenutt entity

class UserHistory : public Updater{
  public:
    const vector<const RecDat*>* get_user_history (int user) const; //returns a vector containing the user's recdats in reverse time order
    void delete_user_history(int user); //user history will be NULL
    void delete_all();
    void clear_user_history(int user); //if user history existed, it will be an empty list
    void clear_all();
    void update(RecDat*) override;
    void write(ostream& file){ throw exception(); }//TODO
    void read(istream& file){ throw exception(); }//TODO
    ~UserHistory(){
      delete_all();
    }
  private:
    vector<vector<const RecDat*>*> user_history_;
};



#endif
