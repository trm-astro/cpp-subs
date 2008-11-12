/*

!!begin
!!title  subs: day of week program
!!author T.R.Marsh
!!descr  Returns day of the week equivalent to a date
!!index  weekday
!!root   weekday
!!class  Programs
!!css    style.css
!!head1  Day of week program

!!emph{weekday} returns the day of the week for a given date.
e.g. weekday "17 Nov 1961"

!!end

*/

#include <string>
#include "trm_subs.h"
#include "trm_date.h"

int main(int argc, char* argv[]){
  try{
    if(argc != 2) throw std::string("usage: weekday date");
    Subs::Date date(argv[1]);
    std::cout << date << " ---> " << date.day_of_week() << std::endl;
  }
  catch(const std::string& message){
    std::cerr << message << std::endl;
  }
}
