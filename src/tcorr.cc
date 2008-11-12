/*

!!begin
!!title  subs: time corrections
!!author T.R.Marsh
!!revised 1 Jan 2008
!!descr  Returns everything to do with times
!!index  tcorr
!!root   tcorr
!!class  Programs
!!css    style.css
!!head1  Time corrections program

!!emph{tcorr} gives all sorts of stuff to do with times for use
on astronomical sources. You give it a source position, telescope
and time and it will return the following the MJD, heliocentric MJD
and barycentric MJD on each of the following timescales UTC, TT, TDB.
The DE405 ephemeris is used.

!!head2 Arguments

!!table
!!arg{position}{Object position, ICRS so just RA and Dec only}
!!arg{telescope}{Name of telescope}
!!arg{time}{UT date/time}
!!table

!!head2 Accuracy

Tests against tempo2 (accurate pulsar timing code) show maximum
deviations of about 100 microsec in the TDB time calculated by this program.
Here are the results of 1000 random positions and times (1990 to 2040)
in units of microsec
<pre>
Mean difference     =    -4.4
RMS difference      =    36.6
Minimum difference  =  -106.2
Maximum difference  =    80.5
10 - 90%-ile range =  -52.2 to  43.4
</pre>

!!end

*/

#include <iomanip>
#include "trm_subs.h"
#include "trm_input.h"
#include "trm_format.h"
#include "trm_time.h"
#include "trm_position.h"
#include "trm_telescope.h"

int main(int argc, char* argv[]) {

    try{

	// Construct Input object
	Subs::Input input(argc, argv, "SUBS_ENV", ".subs");

	// Define inputs

	input.sign_in("position",    Subs::Input::LOCAL, Subs::Input::PROMPT);
	input.sign_in("telescope",   Subs::Input::LOCAL, Subs::Input::PROMPT);
	input.sign_in("time",        Subs::Input::LOCAL, Subs::Input::PROMPT);

	std::string spos, stel, stim;

	input.get_value("position", spos, "13:01:34.45 +00:34:45.3", "position of source (RA Dec Epoch)");
	Subs::Position position(spos);
	input.get_value("telescope", stel, "WHT", "telescope");
	Subs::Telescope telescope(stel);
	input.get_value("time", stim, "29 Sep 2002, 13:01:34.45", "time (UTC)");
	Subs::Time time(stim);

	Subs::Format high(16), medium(10); 
	std::cout << std::endl;
	double mjd   = time.mjd();
	double tt    = time.tt();
	double tdb   = time.tdb(telescope);
	double hcorr = position.tcorr_hel(time,telescope);
	double bcorr = position.tcorr_bar(time,telescope);
	std::cout << " MJD (UTC) = " << high(mjd) << std::endl;
	std::cout << " MJD (TT)  = " << high(tt)  << std::endl;
	std::cout << " MJD (TDB) = " << high(tdb) << std::endl;
	std::cout << "HMJD (UTC) = " << high(mjd + hcorr/86400.)  << std::endl;
	std::cout << "HMJD (TT)  = " << high(tt  + hcorr/86400.)  << std::endl;
	std::cout << "HMJD (TDB) = " << high(tdb + hcorr/86400.)  << std::endl;
	std::cout << "BMJD (UTC) = " << high(mjd + bcorr/86400.)  << std::endl;
	std::cout << "BMJD (TT)  = " << high(tt  + bcorr/86400.)  << std::endl;
	std::cout << "BMJD (TDB) = " << high(tdb + bcorr/86400.)  << std::endl;
    
	std::cout << "Correction to add to get time at heliocentre = " << medium(hcorr) << " seconds." << std::endl;
	std::cout << "Correction to add to get time at barycentre  = " << medium(bcorr) << " seconds." << std::endl;

	Subs::Position sun;
	sun.set_to_sun(time, telescope);

	std::cout << "Position of Sun: " << sun << std::endl;

    }
    catch(const std::string& err){
	std::cerr << err << std::endl;
    }
}
