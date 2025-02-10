// This file is part of GranOO, a workbench for DEM simulation.
//
// Author(s)    : - Damien Andre       IRCER/UNILIM, Limoges France
//                  <damien.andre@unilim.fr>
//                - Jean-luc Charles   Arts et Metiers ParisTech, CNRS, I2M, Bordeaux France
//                  <jean-luc.charles@ensam.eu>
//                - Jeremie Girardot   Arts et Metiers ParisTech, CNRS, I2M, Bordeaux France
//                  <jeremie.girardot@ensam.eu>
//                - Cedric Hubert      LAMIH/UPHF, Valenciennes France
//                  <cedric.hubert@uphf.fr>
//                - Ivan Iordanoff     Arts et Metiers ParisTech, CNRS, I2M, Bordeaux France
//                  <ivan.iordanoff@ensam.eu>
//
// Copyright (C) 2008-2019 D. Andre, JL. Charles, J. Girardot, C. Hubert, I. Iordanoff
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "GranOO3/Core/Problem.hpp"
#include "GranOO3/Core/Domain.hpp"

#include "PlugIn_AddDiscreteElement.hpp"

// Plugin registration definition:
REGISTER_GRANOO_PLUGIN(PlugIn_AddElement);

PlugIn_AddElement::PlugIn_AddElement()
  : Core::PlugInInterface<PlugIn_AddElement>(),
  _x(),
  _y(0.),
  _z(),
  _r(),
  _max_number(1000) {
}

PlugIn_AddElement::~PlugIn_AddElement() {
}

void
PlugIn_AddElement::parse_xml() {

  Core::XmlParser& parser = Core::XmlParser::get();
  double xMax, zMax, rMax;
  parser.read_attribute(Attr::GRANOO_REQUIRED, "xMaxPos", xMax);
  parser.read_attribute(Attr::GRANOO_REQUIRED, "yPos", _y);
  parser.read_attribute(Attr::GRANOO_REQUIRED, "zMaxPos", zMax);
  parser.read_attribute(Attr::GRANOO_REQUIRED, "rMax", rMax);
  parser.read_attribute(Attr::GRANOO_REQUIRED, "MaxNumber", _max_number);

  XmlAssert(xMax > 0., "The xMaxPos attribute must be higher than zero");
  XmlAssert(zMax > 0., "The zMaxPos attribute must be higher than zero");
  XmlAssert(rMax > 0., "The rMax attribute must be higher than zero");

  _x.set_min_max(-fabs(xMax), fabs(xMax));
  _z.set_min_max(-fabs(zMax), fabs(zMax));
  _r.set_min_max(fabs(rMax)- 0.25*fabs(rMax), fabs(rMax));
}

void
PlugIn_AddElement::init() {
}

void
PlugIn_AddElement::run() {
  if (Core::SetOf<DEM::Element >::get().size() < _max_number) {
    const unsigned int maxTry = 1000;
    unsigned int loop = 0;
    do
    {
      const double density = 1000.;
      const double x = _x();
      const double y = _y;
      const double z = _z();
      const double r = _r();
      Geom::Point p(x, y, z);
      if (collide(p, r)==false) {
        new DEM::DiscreteElement(p, r, density);
        break;
      }
      loop++;
    } while (loop < maxTry);
  }
}

bool
PlugIn_AddElement::collide(const Geom::Point& p, const double radius) const {
  Core::SetOf<DEM::DiscreteElement >& set = Core::SetOf<DEM::DiscreteElement >::get();
  for (auto& it: set) {
    Geom::Vector v(p, it->get_center());
    if (v.norm() < radius + it->get_radius())
      return true;
  }
  return false;
}
