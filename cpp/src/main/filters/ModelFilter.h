#ifndef MODELFILTER
#define MODELFILTER

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"
#include "../models/Model.h"

//dokumentacio LASD a TWIKIN is!
//egyik funkcio: predikcio korlatok a kiertekeles gyorsitasara
//minden userre es itemre ad egy korlatot. Az adott userre nem fog a modell nagyobb predikciot adni semmilyen itemre, es megforditva.
//a korlatokat user-korlat vagy item-korlat parokkent adja vissza, korlat szerint csokkenoen rendezve.
//a korlat lehet 0, ez tenyleg 0-t jelent, a listaban nem szereplo userekre/itemekre a korlat implicit 0. Az implicit 0 hasznalnato a modell predikciojanak felulbiralasara (ha az itemek egy szurt halmazan kivanunk kiertekelni).
//a -1 azt jelenti, hogy a filter nem tud korlatot adni az adott itemre/userre, az ilyen userek/itemek a lista elejen szerepelnek tetszoleges sorrendben.
//a visszaadott korlatlista lehet NULL, ez azt jelenti, hogy a filter semmilyen userre/itemre nem ad meg korlatot

//masik funkcio: aktiv itemek szurese kiertekeleskor, ami lehet userfuggo
//ha egy recdatra a szuro hamisat ad vissza, akkor a kiertekelesnek a modell predikciojat a tenyleges erteket felulbiralva 0-nak kell tekinteni
//hasznalat: ha a modell updatelve lett, a run fuggveny hasznalando a frissiteshez

class ModelFilter {
public:
  virtual void run(RecDat* rec_dat){} //run_global es run_personalized?
  virtual void run(double time){RecDat rd; rd.time=time; this->run(&rd);}
  virtual vector<pair<int,double>>* get_global_items(){ return NULL; }
  virtual vector<pair<int,double>>* get_global_users(){ return NULL; }
  virtual vector<pair<int,double>>* get_personalized_items(int user){
    return get_global_items();
  }
  virtual vector<pair<int,double>>* get_personalized_users(int item){
    return get_global_users();
  }
  virtual bool active(RecDat*){ return true; }
  virtual ~ModelFilter(){}
  bool self_test(){
    bool OK = true;
    return OK;
  }
};

#endif
