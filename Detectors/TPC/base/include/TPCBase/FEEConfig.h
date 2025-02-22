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

/// \file FEEConfig.h
/// \brief Frontend electronics configuration values
/// \author Jens Wiechula, Jens.Wiechula@ikf.uni-frankfurt.de

#ifndef AliceO2_TPC_FEEConfig_H_
#define AliceO2_TPC_FEEConfig_H_

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "Rtypes.h"

#include "TPCBase/CalDet.h"
#include "TPCBase/CRU.h"

namespace o2::tpc
{
struct CRUConfig {
  static constexpr int NConfigValues = 7; ///< number of configuration values

  uint32_t linkOn{0};        ///< bitmask of active links
  uint32_t cmcEnabled{0};    ///< if common mode correction is enabled
  uint32_t zsOffset{0};      ///< zero suppression offset value used in ITF
  float itCorr0{1.f};        ///< ion tail scaling parameter
  bool itfEnabled{false};    ///< if ion tail filter correction is enabled
  bool zsEnabled{false};     ///< if zero suppression is enabled
  bool resyncEnabled{false}; ///< if resync feature is enabled

  bool setValues(std::string_view cruData);

  ClassDefNV(CRUConfig, 2);
};

struct FEEConfig {
  /// Tag definitions for TPC/Config/FEE
  enum class Tags : uint8_t {
    Unspecified = 0,         ///< Unspecified
    TestWithZS = 1,          ///< Test configuration with ZS
    Pedestals = 2,           ///< Configuration for Pedestal data taking
    Pulser = 3,              ///< Configuration for Pulser data taking
    Laser = 4,               ///< Configuration for Laser data taking
    Cosmics = 5,             ///< Configuration for Cosmics data taking
    Physics35sigma = 6,      ///< Physics configuration with 3.5 sigma thresholds
    Physics30sigma = 7,      ///< Physics configuration with 3.0 sigma thresholds
    Physics25sigma = 8,      ///< Physics configuration with 2.5 sigma thresholds
    Laser10ADCoff = 9,       ///< Configuration for Laser data taking with 10ADC offset for special studies
    TestNoFilters = 10,      ///< Test configuration without filters
    Physics2025sigma = 11,   ///< Physics configuration with 2.0/2.5 sigma thresholds (IROC/OROC)
    Physics30sigmaPbPb = 12, ///< Physics configuration with 3.0 sigma thresholds for PbPb (using differnet CMC settings wrt. pp config 7)
  };

  enum class PadConfig {
    ITfraction = 0,
    ITexpLambda = 1,
    CMkValues = 2,
    ThresholdMap = 3,
    Pedestals = 4,
  };

  static constexpr size_t MaxLinks = 91 * 36;
  static const std::unordered_map<Tags, const std::string> TagNames;
  static const std::unordered_map<PadConfig, const std::string> PadConfigNames;

  using CalPadMapType = std::unordered_map<std::string, CalPad>;
  FEEConfig() { cruConfig.resize(CRU::MaxCRU); }
  // FEEConfig& operator=(const FEEConfig& config)
  //{
  // padMaps = config.pad
  // return *this;
  //}

  CalPadMapType padMaps;            ///< pad-wise configuration data
  std::vector<CRUConfig> cruConfig; ///< CRU configuration values
  Tags tag = Tags::Unspecified;     ///< tag number

  void clear()
  {
    for ([[maybe_unused]] auto& [key, val] : padMaps) {
      val *= 0;
    }

    for (auto& val : cruConfig) {
      val = CRUConfig();
    }
  }

  size_t getNumberActiveLinks() const;
  bool isCMCEnabled() const;
  bool isITFEnabled() const;
  bool isZSEnabled() const;
  bool isResyncEnabled() const;

  void setAllLinksOn();

  void print() const;
  void printShort() const;

  /// Dead channel map including deactivated links and single channels
  CalDet<bool> getDeadChannelMap() const;

  const CalPad& getPadMap(PadConfig config) const { return padMaps.at(PadConfigNames.at(config)); }

  ClassDefNV(FEEConfig, 2);
};

} // namespace o2::tpc
#endif
