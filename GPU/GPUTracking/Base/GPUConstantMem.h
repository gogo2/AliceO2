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

/// \file GPUConstantMem.h
/// \author David Rohr

#ifndef GPUCONSTANTMEM_H
#define GPUCONSTANTMEM_H

#include "GPUTPCTracker.h"
#include "GPUParam.h"
#include "GPUDataTypes.h"
#include "GPUErrors.h"

#include "GPUTPCGMMerger.h"
#include "GPUTRDTracker.h"

// Dummies for stuff not suppored in legacy code, or for what requires O2 headers while not available
#if defined(GPUCA_HAVE_O2HEADERS)
#include "GPUTPCConvert.h"
#include "GPUTPCCompression.h"
#include "GPUTPCDecompression.h"
#include "GPUITSFitter.h"
#include "GPUTPCClusterFinder.h"
#include "GPUTrackingRefit.h"
#else
#include "GPUO2FakeClasses.h"
#endif

#ifdef GPUCA_KERNEL_DEBUGGER_OUTPUT
#include "GPUKernelDebugOutput.h"
#endif

namespace o2
{
namespace gpu
{
struct GPUConstantMem {
  GPUParam param;
  GPUTPCTracker
    tpcTrackers[GPUCA_NSLICES];
  GPUTPCConvert tpcConverter;
  GPUTPCCompression tpcCompressor;
  GPUTPCDecompression tpcDecompressor;
  GPUTPCGMMerger tpcMerger;
  GPUTRDTrackerGPU trdTrackerGPU;
#ifdef GPUCA_HAVE_O2HEADERS
  GPUTRDTracker trdTrackerO2;
#endif
  GPUTPCClusterFinder tpcClusterer[GPUCA_NSLICES];
  GPUITSFitter itsFitter;
  GPUTrackingRefitProcessor trackingRefit;
  GPUTrackingInOutPointers ioPtrs;
  GPUCalibObjectsConst calibObjects;
  GPUErrors errorCodes;
#ifdef GPUCA_KERNEL_DEBUGGER_OUTPUT
  GPUKernelDebugOutput debugOutput;
#endif

#if defined(GPUCA_HAVE_O2HEADERS)
  template <int32_t I>
  GPUd() auto& getTRDTracker();
#else  // GPUCA_HAVE_O2HEADERS
  template <int32_t I>
  GPUdi() GPUTRDTrackerGPU& getTRDTracker()
  {
    return trdTrackerGPU;
  }
#endif // !GPUCA_HAVE_O2HEADERS
};

#if defined(GPUCA_HAVE_O2HEADERS)
template <>
GPUdi() auto& GPUConstantMem::getTRDTracker<0>()
{
  return trdTrackerGPU;
}
template <>
GPUdi() auto& GPUConstantMem::getTRDTracker<1>()
{
  return trdTrackerO2;
}
#endif

union GPUConstantMemCopyable {
#if !defined(__OPENCL__) || defined(__OPENCL_HOST__)
  GPUh() GPUConstantMemCopyable() {}  // NOLINT: We want an empty constructor, not a default one
  GPUh() ~GPUConstantMemCopyable() {} // NOLINT: We want an empty destructor, not a default one
  GPUh() GPUConstantMemCopyable(const GPUConstantMemCopyable& o)
  {
    for (uint32_t k = 0; k < sizeof(GPUConstantMem) / sizeof(int32_t); k++) {
      ((int32_t*)&v)[k] = ((int32_t*)&o.v)[k];
    }
  }
#endif
  GPUConstantMem v;
};

#if defined(GPUCA_GPUCODE)
static constexpr size_t gGPUConstantMemBufferSize = (sizeof(GPUConstantMem) + sizeof(uint4) - 1);
#endif
} // namespace gpu
} // namespace o2
#if defined(GPUCA_HAS_GLOBAL_SYMBOL_CONSTANT_MEM) && !defined(GPUCA_GPUCODE_HOSTONLY)
GPUconstant() o2::gpu::GPUConstantMemCopyable gGPUConstantMemBuffer;
#endif // GPUCA_HAS_GLOBAL_SYMBOL_CONSTANT_MEM
namespace o2
{
namespace gpu
{

// Must be placed here, to avoid circular header dependency
GPUdi() GPUconstantref() const GPUConstantMem* GPUProcessor::GetConstantMem() const
{
#if defined(GPUCA_GPUCODE_DEVICE) && defined(GPUCA_HAS_GLOBAL_SYMBOL_CONSTANT_MEM) && !defined(GPUCA_GPUCODE_HOSTONLY)
  return &GPUCA_CONSMEM;
#else
  return mConstantMem;
#endif
}

GPUdi() GPUconstantref() const GPUParam& GPUProcessor::Param() const
{
  return GetConstantMem()->param;
}

GPUdi() void GPUProcessor::raiseError(uint32_t code, uint32_t param1, uint32_t param2, uint32_t param3) const
{
  GetConstantMem()->errorCodes.raiseError(code, param1, param2, param3);
}

} // namespace gpu
} // namespace o2

#endif
