#include "GranOO3/Core/Problem.hpp"
#include "GranOO3/Core/Domain.hpp"

#include "PlugIn_RemoteForceInterface.hpp"
#include "MPMD.hpp"
#include "RemoteForceInterface.hpp"

extern MPMDHelper MPMD;

// Plugin registration definition:
REGISTER_GRANOO_PLUGIN(PlugIn_RemoteForceInterface);

PlugIn_RemoteForceInterface::PlugIn_RemoteForceInterface()
  : Core::PlugInInterface<PlugIn_RemoteForceInterface>(), Core::NeedSetOf<GranOO3::DEM::DiscreteElement>(false) {
}

PlugIn_RemoteForceInterface::~PlugIn_RemoteForceInterface() {
}

void
PlugIn_RemoteForceInterface::parse_xml() {
  Core::XmlParser& parser = Core::XmlParser::get();
  parser.read_attribute(Attr::GRANOO_REQUIRED, "calculator", calculator_name);
}

void
PlugIn_RemoteForceInterface::init() {
    int ret;
    RFI.name = "GRANOO";
    MPMDIntercomm inter = MPMD[calculator_name];
    if (inter) {
        ret = RFI.Connect(MPMD.work, inter.work);
        //if (ret) TODO:ERROR;
    } else {
        fprintf(stderr, "Didn't find TCLB in MPMD\n");
        // TODO:ERROR;
    }
    first_print = true;
    wsize.resize(RFI.Workers());
    windex.resize(RFI.Workers());
    domain_length[0] = 1.0;
    domain_length[1] = 1.0;
    domain_length[2] = 1.0;
    domain_periodicity[0] = false;
    domain_periodicity[1] = false;
    domain_periodicity[2] = false;
}

void
PlugIn_RemoteForceInterface::run() {
    if (!RFI.Active()) return;

    auto& set = get_setof();

    if (first_print) {
        printf("GRANOO: RFI.Rot:%s\n",
            RFI.Rot() ? "true" : "false"
        );
        first_print = false;
    }

    for (int phase=0; phase<3; phase++) {
        if (phase == 0) {
            for (int i = 0; i < RFI.Workers(); i++) wsize[i] = 0;
        } else {
            for (int i = 0; i < RFI.Workers(); i++) windex[i] = 0;
        }
        for (auto de : set) {
            auto& sph = *de;
            
            double r = sph.get_radius();
            const Geom::Vector& pos = sph.get_position();
            const Geom::Vector& v = sph.get_linear_velocity();
            const Geom::Vector& omega = sph.get_angular_velocity();
            Geom::Vector& f = sph.force();
            Geom::Vector& torque = sph.torque();
            int minper[3], maxper[3], d[3];
            size_t offset = 0;
            for (int worker = 0; worker < RFI.Workers(); worker++) {
                double x[3] = {pos.x(), pos.y(), pos.z()};
                
                for (int j=0; j<3; j++) {
                    double prd = domain_length[j];
                    double lower = 0;
                    double upper = domain_length[j];
                    if (RFI.WorkerBox(worker).declared) {
                        lower = RFI.WorkerBox(worker).lower[j];
                        upper = RFI.WorkerBox(worker).upper[j];
                    }
                    if (domain_periodicity[j]) {
                        maxper[j] = floor((upper - x[j] + r)/prd);
                        minper[j] = ceil((lower - x[j] - r)/prd);
                    } else {
                        if ((x[j] + r >= lower) && (x[j] - r <= upper)) {
                            minper[j] = 0;
                            maxper[j] = 0;
                        } else {
                            minper[j] = 0;
                            maxper[j] = -1;  // no balls
                        }
                    }
                    //       printf("particle %ld dimenstion %d in %d worker interval [%lg %lg] and periodicity %lg: %lg copied %d:%d\n", k, j, worker, lower, upper, prd, x[j], minper[j], maxper[j]);
                }

                int copies = (maxper[0] - minper[0] + 1)*(maxper[1] - minper[1] + 1)*(maxper[2] - minper[2] + 1);
                //     if (copies > 1) printf("particle %ld is copied %d times (%d %d)x(%d %d)x(%d %d)\n", k, copies, minper[0], maxper[0], minper[1], maxper[1], minper[2], maxper[2]);
                for (d[0] = minper[0]; d[0] <= maxper[0]; d[0]++) {
                    for (d[1] = minper[1]; d[1] <= maxper[1]; d[1]++) {
                        for (d[2] = minper[2]; d[2] <= maxper[2]; d[2]++) {
                            double px[3];
                            for (int j=0; j<3; j++) px[j] = x[j] + d[j]*domain_length[j];
                            if (phase == 0) {
                                wsize[worker]++;
                            } else {
                                size_t i = offset + windex[worker];
                                if (phase == 1) {
                                    //printf("particle %ld sent %d at index %ld\n", k, worker, i);
                                    RFI.setData(i, RFI_DATA_R, r);
                                    RFI.setData(i, RFI_DATA_POS + 0, px[0]);
                                    RFI.setData(i, RFI_DATA_POS + 1, px[1]);
                                    RFI.setData(i, RFI_DATA_POS + 2, px[2]);
                                    RFI.setData(i, RFI_DATA_VEL + 0, v.x());
                                    RFI.setData(i, RFI_DATA_VEL + 1, v.y());
                                    RFI.setData(i, RFI_DATA_VEL + 2, v.z());
                                    if (RFI.Rot()) {
                                        RFI.setData(i, RFI_DATA_ANGVEL + 0, omega.x());
                                        RFI.setData(i, RFI_DATA_ANGVEL + 1, omega.y());
                                        RFI.setData(i, RFI_DATA_ANGVEL + 2, omega.z());
                                    }
                                } else {
                                    f.x() += RFI.getData(i, RFI_DATA_FORCE + 0);
                                    f.y() += RFI.getData(i, RFI_DATA_FORCE + 1);
                                    f.z() += RFI.getData(i, RFI_DATA_FORCE + 2);
                                    torque.x() += RFI.getData(i, RFI_DATA_MOMENT + 0);
                                    torque.y() += RFI.getData(i, RFI_DATA_MOMENT + 1);
                                    torque.z() += RFI.getData(i, RFI_DATA_MOMENT + 2);
                                }
                                windex[worker]++;
                            }
                        }
                    }
                }
                offset += wsize[worker];
            }
        }
        if (phase == 0) {
            for (int worker = 0; worker < RFI.Workers(); worker++) RFI.Size(worker) = wsize[worker];
            //printf("sizes:"); for (int worker = 0; worker < RFI.Workers(); worker++) printf(" %ld", (size_t) wsize[worker]); printf("\n");
            RFI.SendSizes();
            RFI.Alloc();
        } else if (phase == 1) {
            //printf("indexes:"); for (int worker = 0; worker < RFI.Workers(); worker++) printf(" %ld", (size_t) windex[worker]); printf("\n");
            RFI.SendParticles();
            RFI.SendForces();
        } else {
        }
    }


}
