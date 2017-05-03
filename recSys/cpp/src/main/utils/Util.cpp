/*
 * util.cpp
 *
 *  Created on: 2014.03.19.
 *      Author: kisstom
 */

#include "Util.h"
#include <math.h>

void Util::sum_update_with(vector<double>* updateFactor,
    vector<double>* otherFactor, double coef1, double coef2) {
  if (NULL == updateFactor || NULL == otherFactor) return;

  for (int i = 0; i < (int) updateFactor->size(); ++i) {
    (*updateFactor)[i] = (*updateFactor)[i] * coef1 + (*otherFactor)[i] * coef2;
  }
}

int Util::euclideanAlg(int a, int b) {
  int temp;

  while (1) {
    if (b > a) {
      temp = a;
      a = b;
      b = temp;
    }

    if (b == 0) {
      break;
    }

    temp = a;
    a = b;
    b = temp % b;
  }

  return a;
}

int Util::firstRelPrimeTo(int a) {
  if (2 == a) return 1;

  for (int i = 2; i < a; ++i) {
    if (1 == euclideanAlg(a, i)) {
      return i;
    }
  }

  return -1;
}

double Util::computeGamma(double x, double y, double tau){
  //cerr << "Util::computeGamma(x=" << x << ",y=" << y << ",tau=" << tau << ")" << endl;
  double deltaT = x - y;
  double weight = 1;
  if (deltaT > 0) weight = 1.0 - (log(deltaT)/log(tau));
  return weight;
}

void Util::printNorm(int i, vector<double>* v) {
  printf("%d", i);

  double norm = 0.0;
  for (int j = 0; j < (int) v->size(); ++j) {
    norm += (*v)[j] * (*v)[j];
  }

  printf(" %lf\n", sqrt(norm));
}

void Util::printVector(int i, vector<double>* v) {
  printf("%d", i);

  for (int j = 0; j < (int) v->size(); ++j) {
    printf (" %0.10lf", (*v)[j]);
  }

  printf("\n");
}

