//-------------------  distorted_mapped_cells_01.cc  ------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2013 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//-------------------  distorted_mapped_cells_01.cc  ------------------------

// Test that an assertion is thrown when MappingQ distorts the geometry too
// much on a thin hyper shell

#include "../tests.h"
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/fe/mapping_q.h>
#include <deal.II/fe/fe_nothing.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria_boundary_lib.h>


void test()
{
  const int dim = 2;
  Triangulation<dim> tria(Triangulation<dim>::none,true);

  // shell is so narrow that MappingQ(2) distorts the cell
  GridGenerator::quarter_hyper_shell (tria, Point<dim>(), 0.95, 1, 1);
  static HyperShellBoundary<dim> boundary;
  tria.set_boundary (0, boundary);

  FE_Nothing<dim> dummy;
  MappingQ<dim> mapping(2);
  QGauss<dim> quad(2);
  FEValues<dim> fe_val (mapping, dummy, quad, update_JxW_values);
  double integral = 0.;
  /*typename*/ Triangulation<dim>::active_cell_iterator
    cell = tria.begin_active(), endc = tria.end();
  for ( ; cell != endc; ++cell)
    {
      fe_val.reinit (cell);
      for (unsigned int q=0; q<quad.size(); ++q)
        integral += fe_val.JxW(q);
    }
  deallog << "Integral = " << integral << std::endl;
}


int
main()
{
  deal_II_exceptions::disable_abort_on_exception();
  std::ofstream logfile ("distorted_mapped_cells_02/output");
  deallog << std::setprecision(4) << std::fixed;
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  test();

  return 0;
}


