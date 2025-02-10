#ifndef _PlugIn_RemoteForceInterface_hpp_
#define _PlugIn_RemoteForceInterface_hpp_

#include "GranOO3/Common.hpp"

#include "GranOO3/Core/PlugIn.hpp"
#include "GranOO3/Math/UniRandom.hpp"

#include "RemoteForceInterface.hpp"

typedef rfi::RemoteForceInterface<rfi::ForceIntegrator, rfi::RotParticle, rfi::ArrayOfStructures, real_t> RFI_t;

class PlugIn_RemoteForceInterface : public Core::PlugInInterface<PlugIn_RemoteForceInterface>, public Core::NeedSetOf<Physic::Node>
{
  RFI_t RFI;
public:
  DECLARE_CUSTOM_GRANOO_PLUGIN(RemoteForceInterface);
  ~PlugIn_RemoteForceInterface();
  PlugIn_RemoteForceInterface();
  void parse_xml();
  void init();
  void run();
};

#endif
