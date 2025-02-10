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
//



#ifndef _PlugIn_AddElement_hpp_
#define _PlugIn_AddElement_hpp_

//
// Using the GranOO API:
//
#include "GranOO3/Common.hpp"

#include "GranOO3/Core/PlugIn.hpp"
#include "GranOO3/Math/UniRandom.hpp"

class PlugIn_AddElement : public Core::PlugInInterface<PlugIn_AddElement>
{

public:
  // Macro used to register a user plugin with its Id:
  DECLARE_CUSTOM_GRANOO_PLUGIN(AddElement);

  // A plugin MUST declare its constructor and destructor:
  ~PlugIn_AddElement();
  PlugIn_AddElement();

  // A plugin MUST declare the 3 methods run(), parse_xml() and init()
  // (see definition of these methods in .cpp file):
  void parse_xml();
  void init();
  void run();

public:
  bool collide(const Geom::Point& p, const double radius) const;

private:
  Math::UniRandom _x;
  double _y;
  Math::UniRandom _z;
  Math::UniRandom _r;
  unsigned int _max_number;
};

#endif
