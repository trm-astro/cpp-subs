/*

!!begin
!!title  subs: interpolation program
!!author T.R.Marsh
!!descr  Interpolates tabular data onto uniform grid
!!index  interp
!!root   interp
!!class  Programs
!!css    style.css
!!head1  Interpolation program

!!emph{interp} interpolates irregularly spaced but monotonic two-column 
data onto a uniform array. 

!!table
!!arg{file}{Two-column file to load.}
!!arg{x1}{First X value of regular output array}
!!arg{x2}{Last X value of regular output array}
!!arg{nx}{Number of X values}
!!table

!!end

*/

#include <climits>
#include <cfloat>
#include <string>
#include <iostream>
#include "trm_subs.h"
#include "trm_array1d.h"
#include "trm_input.h"

using Subs::operator+;

int main(int argc, char* argv[]){
  try{

    // Construct Input object
    Subs::Input input(argc, argv, "SUBS_ENV", ".subs");

    // Define inputs

    input.sign_in("file",    Subs::Input::LOCAL, Subs::Input::PROMPT);
    input.sign_in("x1",      Subs::Input::LOCAL, Subs::Input::PROMPT);
    input.sign_in("x2",      Subs::Input::LOCAL, Subs::Input::PROMPT);
    input.sign_in("nx",      Subs::Input::LOCAL, Subs::Input::PROMPT);

    std::string file;
    input.get_value("file", file, "input_file", "file of two-column data");
    double x1;
    input.get_value("x1", x1, 0., -DBL_MAX, DBL_MAX, "first X value of output array");
    double x2;
    input.get_value("x2", x2, 100., -DBL_MAX, DBL_MAX, "last X value of output array");
    int nx;
    input.get_value("nx", nx, 100, 2, INT_MAX, "number of X values");

    // Read in data

    Subs::Array1D<Subs::xy<double,double> > twocol(file);

    // Separate out
    double xarr[twocol.size()], yarr[twocol.size()];
    for(int i=0; i<twocol.size(); i++){
      xarr[i] = twocol[i].x;
      yarr[i] = twocol[i].y;
    }
    
    long unsigned int jhi = twocol.size()/2;
    double x, y;
    for(int i=0; i<nx; i++){
      x = x1 + (x2-x1)*i/double(nx-1);
      Subs::hunt(xarr, twocol.size(), x, jhi);
      if(jhi == 0 || int(jhi) == twocol.size())
	throw std::string("Error: Trying to interpolate out of range of input file. Value = " + Subs::str(x));
      y = ((x-xarr[jhi-1])*yarr[jhi]+(xarr[jhi]-x)*yarr[jhi-1])/(xarr[jhi]-xarr[jhi-1]);
      std::cout << x << " " << y << "\n";
    }
  }
  catch(const std::string& message){
    std::cerr << message << std::endl;
  }
}
