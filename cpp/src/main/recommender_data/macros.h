#define USER int
#define ITEM int
#define SCORE double
#define RANK int

#define EXPAND_VECTORMAP(vecmap, pos) if ((int)(vecmap).size()<=(pos)) { (vecmap).resize((pos)+1); };
#define PUT_VECTORMAP(vecmap, pos, value) if ((int)(vecmap).size()<=(pos)) { (vecmap).resize((pos)+1); }; (vecmap)[(pos)]=(value);
#define ADD_VECTORMAP(vecmap, pos, value) if ((int)(vecmap).size()<=(pos)) { (vecmap).resize((pos)+1); }; (vecmap)[(pos)]+=(value);
#define GET_VECTORMAP(vecmap, pos, default) (((int)(vecmap).size()<=(pos)) ? default : (vecmap)[(pos)])

#define PUT_VECVECMAP(vecvecmap, pos, value) if ((int)(vecvecmap).size()<=(pos)) { (vecvecmap).resize((pos)+1); }; (vecvecmap)[(pos)].push_back((value));
