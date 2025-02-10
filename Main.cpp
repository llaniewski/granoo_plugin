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
// Main for generic GranOO application
//

#include <mpi.h>
#include "GranOO3/Common.hpp"
#include "MPMD.hpp"

MPMDHelper MPMD;

int main(int argc, char * argv[]) {
  MPI_Init(&argc, &argv);
  MPMD.Init(MPI_COMM_WORLD,"GRANOO");
  MPMD.Identify();
  Core::Problem::get().run(argc, argv);
  MPI_Finalize();
  return 0;
}
