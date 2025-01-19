// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file GPUTPCGMPolynomialFieldManager.cxx
/// \author Sergey Gorbunov, David Rohr

#include "GPUCommonConstants.h"
#include "GPUTPCGMPolynomialFieldManager.h"
#include "GPUTPCGMPolynomialField.h"
#include <cmath>

using namespace o2::gpu;

int32_t GPUTPCGMPolynomialFieldManager::GetPolynomialField(StoredField_t fieldType, float nominalFieldkG, GPUTPCGMPolynomialField& field)
{
  //
  // get pre-calculated polynomial field approximation of the TPC region
  // returns -1 when the polynomial field is not exist
  // returns -2 if number of coefficients in GPUTPCGMPolynomialField is not 10
  //

  const int32_t kTpcM = GPUTPCGMPolynomialField::NTPCM;
  const int32_t kTrdM = GPUTPCGMPolynomialField::NTRDM;
  const int32_t kItsM = GPUTPCGMPolynomialField::NITSM;

  //
  // polynomial coefficients for the Uniform Bz field
  //
  float kSolUBx[100], kSolUBy[100], kSolUBz[100];
  for (int32_t i = 0; i < 100; i++) {
    kSolUBx[i] = 0;
    kSolUBy[i] = 0;
    kSolUBz[i] = 0;
  }
  kSolUBz[0] = 1;

  //
  // polynomial coefficients for 2kG field
  //

  const float kTpcSol2Bx[kTpcM] = {8.25026654638e-06, 2.73111226079e-07, 8.09913785815e-07, -4.43062708655e-06, -1.12499973781e-08, 3.94054833208e-09, 2.66427264251e-07, -6.30059693307e-09, 2.79869932784e-10, 1.15630518494e-08};

  const float kTpcSol2By[kTpcM] = {-1.62876094691e-04, 8.20370075871e-07, -2.60450360656e-06, 5.25321956957e-06, 1.18615373079e-09, -1.44053808881e-08, 1.92043728142e-10, -2.99749697286e-10, 2.66646878799e-07, -1.15439746651e-09};

  const float kTpcSol2Bz[kTpcM] = {9.99487757683e-01, -5.67969527765e-06, 4.76676314065e-06, 2.46677245741e-06, 1.46798569745e-07, 5.39603639549e-10, 2.86027042051e-08, 1.45939324625e-07, -2.48197662422e-09, -2.47860867830e-07};

  //
  // polynomial coefficients for 5kG field
  //

  const float kTpcSol5Bx[kTpcM] = {-2.58322252193e-05, 2.25564940592e-06, -4.14718357433e-08, -2.75251750281e-06, -8.72029382037e-09, 1.72417402577e-09, 3.19352068345e-07, -3.28086002810e-09, 5.64790381130e-10, 8.92192542068e-09};
  const float kTpcSol5By[kTpcM] = {6.37950097371e-06, -4.46194050596e-08, 9.01212274584e-07, 8.26001087262e-06, 7.99017740860e-10, -7.45108241773e-09, 4.81764572680e-10, 8.35443714209e-10, 3.14677095048e-07, -1.18421328299e-09};
  const float kTpcSol5Bz[kTpcM] = {9.99663949013e-01, -3.54553162651e-06, 7.73496958573e-06, -2.90551361104e-06, 1.69738939348e-07, 5.00871899511e-10, 2.10037196524e-08, 1.66827078374e-07, -2.64136179595e-09, -3.02637317873e-07};

  //
  // TRD: polynomial coefficients for 2kG field
  //

  const float kTrdSol2Bx[kTrdM] = {1.39999421663e-04, 3.72149628447e-07, 6.76831518831e-07, -4.61197259938e-06, -9.38696409492e-09, 2.51344522972e-09, 2.28966001714e-07, -4.12119849358e-09, 4.61481075575e-10, 2.85501511321e-09,
                                   2.10280165676e-12, 3.08102219952e-12, 5.71178174202e-11, -1.15748409121e-11, -1.05804167511e-11, -9.36777890502e-13, 1.40891139901e-12, 2.92545414976e-12, -1.46659052090e-12, -6.02929435978e-13};

  const float kTrdSol2By[kTrdM] = {-1.99000875000e-04, 6.84237363657e-07, -2.77501658275e-06, 4.26194901593e-06, 1.74802150532e-09, -1.41377940466e-08, 5.89200521706e-10, 1.92612537031e-10, 2.27884683568e-07, -2.04284839045e-10,
                                   2.09083249846e-12, -6.42724241884e-12, -6.17209018269e-12, 3.06769562010e-12, 4.07716950479e-11, -2.30143703574e-12, 4.39658427937e-13, 6.33123345417e-11, 5.73038535026e-12, -9.96955035887e-12};

  const float kTrdSol2Bz[kTrdM] = {1.00137376785e+00, -6.18833337285e-06, 4.96962411489e-06, 3.53747350346e-06, 1.05127497818e-07, 3.99420441166e-10, 2.07503472183e-08, 1.03241909244e-07, -2.10957140645e-09, -2.04966426054e-07,
                                   3.83026041439e-11, -1.67644596122e-11, -6.03145658418e-12, 2.44712149849e-11, -2.76764136782e-12, -5.21652547547e-11, 2.43306919750e-11, -8.09586213579e-15, -4.60759208565e-11, -9.12051337232e-12};

  //
  // TRD: polynomial coefficients for 5kG field
  //

  const float kTrdSol5Bx[kTrdM] = {6.89610242262e-05, 2.17151045945e-06, -7.68707906218e-08, -3.13707118949e-06, -7.96343080367e-09, 1.75736514230e-09, 2.83481057295e-07, -2.42189890365e-09, 4.98622587664e-10, 3.90359566893e-09,
                                   -2.40058146972e-12, -1.27837779425e-12, 4.07061988283e-11, -8.92471806093e-13, -6.87322030887e-12, 3.32263079897e-12, 1.05860131316e-13, 3.55080006197e-12, 1.63436191664e-12, -2.12673181474e-13};

  const float kTrdSol5By[kTrdM] = {-1.91418548638e-05, -7.97522119456e-08, 8.38440655571e-07, 7.00077862348e-06, 7.66234908856e-10, -8.10954858821e-09, 4.48277082121e-10, 7.65219609900e-10, 2.77397276705e-07, -3.89592108574e-10,
                                   -1.96872805059e-13, -9.82137114691e-13, -2.22295294151e-12, -1.64837300710e-13, 2.61398229451e-11, 1.68494536899e-12, -2.94431232867e-12, 6.14056860915e-11, 3.23249218191e-12, -6.08022182949e-12};

  const float kTrdSol5Bz[kTrdM] = {1.00182890892e+00, -4.07937841373e-06, 7.91169622971e-06, -7.57556847475e-07, 1.29350567590e-07, 5.66281244119e-10, 1.67468972023e-08, 1.25723317979e-07, -2.22481455481e-09, -2.68792632596e-07,
                                   2.65291157098e-11, -1.09183417515e-11, -6.78487170960e-12, 1.72749713839e-11, 2.80368957217e-12, -3.49344546346e-11, 2.45735688742e-11, -6.87686713130e-12, -4.55244418551e-11, -1.83581587432e-11};

  //
  // ITS: polynomial coefficients for 2kG field
  //

  const float kItsSol2Bx[kItsM] = {-5.91223470110e-05, 6.90032351258e-08, 8.25885251743e-07, -5.32185140401e-06, -9.65053725821e-09, 1.08244559982e-08, 2.85320879811e-07, -5.09609510146e-09, -6.39149955184e-10, 1.49089558477e-08};

  const float kItsSol2By[kItsM] = {-1.48597624502e-04, 8.25465463095e-07, -2.97493147627e-06, 4.98509780300e-06, 4.24987989245e-10, -1.98144327612e-08, -1.20077459087e-09, -1.19401033505e-09, 2.82514832861e-07, -1.50957846223e-09};

  const float kItsSol2Bz[kItsM] = {9.99939501286e-01, -6.24587028142e-06, 4.81351708004e-06, 2.35402990256e-06, 1.26109355847e-07, 9.08826292001e-10, 3.43317942963e-08, 1.25566515408e-07, -2.71147437836e-09, -2.69678821496e-07};

  //
  // ITS: polynomial coefficients for 5kG field
  //

  const float kItsSol5Bx[kItsM] = {-6.05255008850e-05, 2.42733176492e-06, -9.46944425095e-08, -3.22587538903e-06, -8.23875456746e-09, 2.83529777434e-09, 3.42276649690e-07, -3.80562692470e-09, 3.64589525237e-10, 1.08676720956e-08};

  const float kItsSol5By[kItsM] = {7.84835810919e-06, 5.19906029339e-09, 9.49704599407e-07, 7.98410110292e-06, 1.74484093840e-09, -7.61669749494e-09, 5.19390808140e-10, 9.18893627855e-10, 3.37364895131e-07, -1.46239598209e-09};

  const float kItsSol5Bz[kItsM] = {1.00001418591e+00, -3.69126610167e-06, 7.76097112976e-06, -3.11396547659e-06, 1.64195810726e-07, 2.47078468796e-10, 2.39289423831e-08, 1.61199579907e-07, -3.16838866254e-09, -3.23542707292e-07};

  field.Reset();

  // check if GPUTPCGMPolynomialField class is ok

  if (GPUTPCGMPolynomialField::NTPCM != kTpcM) {
    return -2;
  }

  // check which field map is in use

  const float *cTpcBx, *cTpcBy, *cTpcBz, *cTrdBx, *cTrdBy, *cTrdBz, *cItsBx, *cItsBy, *cItsBz;

  double nominalBz = nominalFieldkG * gpu_common_constants::kCLight;

  if (fieldType == GPUTPCGMPolynomialFieldManager::kUniform) {
    cTpcBx = kSolUBx;
    cTpcBy = kSolUBy;
    cTpcBz = kSolUBz;
    cTrdBx = kSolUBx;
    cTrdBy = kSolUBy;
    cTrdBz = kSolUBz;
    cItsBx = kSolUBx;
    cItsBy = kSolUBy;
    cItsBz = kSolUBz;
  } else if (fieldType == GPUTPCGMPolynomialFieldManager::k2kG) {
    cTpcBx = kTpcSol2Bx;
    cTpcBy = kTpcSol2By;
    cTpcBz = kTpcSol2Bz;
    cTrdBx = kTrdSol2Bx;
    cTrdBy = kTrdSol2By;
    cTrdBz = kTrdSol2Bz;
    cItsBx = kItsSol2Bx;
    cItsBy = kItsSol2By;
    cItsBz = kItsSol2Bz;
  } else if (fieldType == GPUTPCGMPolynomialFieldManager::k5kG) {
    cTpcBx = kTpcSol5Bx;
    cTpcBy = kTpcSol5By;
    cTpcBz = kTpcSol5Bz;
    cTrdBx = kTrdSol5Bx;
    cTrdBy = kTrdSol5By;
    cTrdBz = kTrdSol5Bz;
    cItsBx = kItsSol5Bx;
    cItsBy = kItsSol5By;
    cItsBz = kItsSol5Bz;
  } else { // field is not known
    return -1;
  }

  float TpcBx[kTpcM], TpcBy[kTpcM], TpcBz[kTpcM];
  for (int32_t i = 0; i < kTpcM; i++) {
    TpcBx[i] = nominalBz * cTpcBx[i];
    TpcBy[i] = nominalBz * cTpcBy[i];
    TpcBz[i] = nominalBz * cTpcBz[i];
  }

  float TrdBx[kTrdM], TrdBy[kTrdM], TrdBz[kTrdM];
  for (int32_t i = 0; i < kTrdM; i++) {
    TrdBx[i] = nominalBz * cTrdBx[i];
    TrdBy[i] = nominalBz * cTrdBy[i];
    TrdBz[i] = nominalBz * cTrdBz[i];
  }

  float ItsBx[kItsM], ItsBy[kItsM], ItsBz[kItsM];
  for (int32_t i = 0; i < kItsM; i++) {
    ItsBx[i] = nominalBz * cItsBx[i];
    ItsBy[i] = nominalBz * cItsBy[i];
    ItsBz[i] = nominalBz * cItsBz[i];
  }

  field.SetFieldNominal(nominalBz);
  field.SetFieldTpc(TpcBx, TpcBy, TpcBz);
  field.SetFieldTrd(TrdBx, TrdBy, TrdBz);
  field.SetFieldIts(ItsBx, ItsBy, ItsBz);

  return 0;
}

int32_t GPUTPCGMPolynomialFieldManager::GetPolynomialField(float nominalFieldkG, GPUTPCGMPolynomialField& field)
{
  //
  // get closest pre-calculated polynomial field approximation of the TPC region  for the given field value nominalFieldkG
  // returns !=0 in case of error
  //
  // check which field map is in use

  field.Reset();

  StoredField_t type = kUnknown;

  if (fabsf(nominalFieldkG) < 0.01f) {
    type = kUniform;
    nominalFieldkG = 0;
  } else if (fabsf(fabsf(nominalFieldkG) - 5.00668f) <= fabsf(fabsf(nominalFieldkG) - 2.f)) {
    type = k5kG;
  } else {
    type = k2kG;
  }

  return GetPolynomialField(type, nominalFieldkG, field);
}
