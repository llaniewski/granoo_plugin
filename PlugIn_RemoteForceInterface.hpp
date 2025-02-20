#ifndef _PlugIn_RemoteForceInterface_hpp_
#define _PlugIn_RemoteForceInterface_hpp_

#include "GranOO3/Common.hpp"

#include "GranOO3/Core/PlugIn.hpp"
#include "GranOO3/Math/UniRandom.hpp"

#include "RemoteForceInterface.h"

typedef double real_t;
typedef rfi::RemoteForceInterface<rfi::ForceIntegrator, rfi::RotParticle, rfi::ArrayOfStructures, real_t> RFI_t;

class PlugIn_RemoteForceInterface : public Core::PlugInInterface<PlugIn_RemoteForceInterface>, public Core::NeedSetOf<GranOO3::DEM::DiscreteElement>
{
  RFI_t RFI;
  std::vector<size_t> wsize;
  std::vector<size_t> windex;
  bool first_print;
  std::string calculator_name;
  bool domain_periodicity[3];
  double domain_length[3];
  int subiter, subiters;
  std::vector<Geom::Vector> forces;
  std::vector<Geom::Vector> torques;
public:
  DECLARE_CUSTOM_GRANOO_PLUGIN(RemoteForceInterface);
  ~PlugIn_RemoteForceInterface();
  PlugIn_RemoteForceInterface();
  void parse_xml();
  void init();
  void run();
};

#endif
