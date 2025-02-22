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
//
// This file has been generated. Do not modify it by hand or your changes might
// be lost.
//
#include "CathodeSegmentationCreator.h"

namespace o2
{
namespace mch
{
namespace mapping
{
namespace impl4
{
CathodeSegmentation* createSegType16(bool isBendingPlane)
{
  if (isBendingPlane) {
    return new CathodeSegmentation{
      16,
      true,
      /* PG */
      {{3, 3, 0, 0, -20},
       {4, 3, 0, 20, -20},
       {7, 3, 0, -40, -20},
       {8, 3, 0, -20, -20},
       {101, 0, 0, 20, -4},
       {102, 2, 0, 10, 4},
       {103, 1, 0, 0, -4},
       {106, 0, 0, -20, -4},
       {107, 2, 0, -30, 4},
       {108, 1, 0, -40, -4}},
      /* PGT */
      {/* L10 */ {2, 48, {35, 36, 38, 33, 41, 34, 44, 37, 45, 32, 47, 39, 50, 40, 51, 42, 53, 43, 56, 46, 57, 48, 59, 49, 60, 52, 61, 54, 62, 55, 63, 58, -1, 31, -1, 30, -1, 29, -1, 28, -1, 27, -1, 26, -1, 24, -1, 23, -1, 20, -1, 21, -1, 16, -1, 19, -1, 12, -1, 14, -1, 11, -1, 13, -1, 7, -1, 8, -1, 5, -1, 2, -1, 6, -1, 1, -1, 3, -1, 0, -1, 4, -1, 9, -1, 10, -1, 15, -1, 17, -1, 18, -1, 22, -1, 25}},
       /* L9 */ {2, 48, {13, 7, 11, 8, 14, 5, 12, 2, 19, 6, 16, 1, 21, 3, 20, 0, 23, 4, 24, 9, 26, 10, 27, 15, 28, 17, 29, 18, 30, 22, 31, 25, 58, -1, 55, -1, 54, -1, 52, -1, 49, -1, 48, -1, 46, -1, 43, -1, 42, -1, 40, -1, 39, -1, 32, -1, 37, -1, 34, -1, 33, -1, 36, -1, 35, -1, 38, -1, 41, -1, 44, -1, 45, -1, 47, -1, 50, -1, 51, -1, 53, -1, 56, -1, 57, -1, 59, -1, 60, -1, 61, -1, 62, -1, 63, -1}},
       /* O11 */ {2, 32, {58, 31, 55, 30, 54, 29, 52, 28, 49, 27, 48, 26, 46, 24, 43, 23, 42, 20, 40, 21, 39, 16, 32, 19, 37, 12, 34, 14, 33, 11, 36, 13, 35, 7, 38, 8, 41, 5, 44, 2, 45, 6, 47, 1, 50, 3, 51, 0, 53, 4, 56, 9, 57, 10, 59, 15, 60, 17, 61, 18, 62, 22, 63, 25}},
       /* O12 */ {2, 32, {25, 63, 22, 62, 18, 61, 17, 60, 15, 59, 10, 57, 9, 56, 4, 53, 0, 51, 3, 50, 1, 47, 6, 45, 2, 44, 5, 41, 8, 38, 7, 35, 13, 36, 11, 33, 14, 34, 12, 37, 19, 32, 16, 39, 21, 40, 20, 42, 23, 43, 24, 46, 26, 48, 27, 49, 28, 52, 29, 54, 30, 55, 31, 58}}},
      /* PS */
      {{10, 0.5}}};
  } else {
    return new CathodeSegmentation{
      16,
      false,
      /* PG */
      {{1025, 3, 0, 20, -20},
       {1026, 2, 0, 0, -20},
       {1029, 3, 0, -20, -20},
       {1030, 2, 0, -40, -20},
       {1128, 0, 0, 0, 0},
       {1129, 1, 0, 20, 0},
       {1133, 0, 0, -40, 0},
       {1134, 1, 0, -20, 0}},
      /* PGT */
      {/* O5 */ {28, 2, {25, 18, 15, 9, 0, 1, 2, 8, 13, 14, 19, 21, 23, 26, 28, 30, 58, 54, 49, 46, 42, 39, 37, 33, 35, 41, 45, 50, 22, 17, 10, 4, 3, 6, 5, 7, 11, 12, 16, 20, 24, 27, 29, 31, 55, 52, 48, 43, 40, 32, 34, 36, 38, 44, 47, 51}},
       /* O6 */ {28, 2, {0, 1, 2, 8, 13, 14, 19, 21, 23, 26, 28, 30, 58, 54, 49, 46, 42, 39, 37, 33, 35, 41, 45, 50, 53, 57, 60, 62, 3, 6, 5, 7, 11, 12, 16, 20, 24, 27, 29, 31, 55, 52, 48, 43, 40, 32, 34, 36, 38, 44, 47, 51, 56, 59, 61, 63}},
       /* O7 */ {28, 2, {63, 61, 59, 56, 51, 47, 44, 38, 36, 34, 32, 40, 43, 48, 52, 55, 31, 29, 27, 24, 20, 16, 12, 11, 7, 5, 6, 3, 62, 60, 57, 53, 50, 45, 41, 35, 33, 37, 39, 42, 46, 49, 54, 58, 30, 28, 26, 23, 21, 19, 14, 13, 8, 2, 1, 0}},
       /* O8 */ {28, 2, {51, 47, 44, 38, 36, 34, 32, 40, 43, 48, 52, 55, 31, 29, 27, 24, 20, 16, 12, 11, 7, 5, 6, 3, 4, 10, 17, 22, 50, 45, 41, 35, 33, 37, 39, 42, 46, 49, 54, 58, 30, 28, 26, 23, 21, 19, 14, 13, 8, 2, 1, 0, 9, 15, 18, 25}}},
      /* PS */
      {{0.714285714, 10}}};
  }
}
class CathodeSegmentationCreatorRegisterCreateSegType16
{
 public:
  CathodeSegmentationCreatorRegisterCreateSegType16()
  {
    registerCathodeSegmentationCreator(16, createSegType16);
  }
} aCathodeSegmentationCreatorRegisterCreateSegType16;

} // namespace impl4
} // namespace mapping
} // namespace mch
} // namespace o2
