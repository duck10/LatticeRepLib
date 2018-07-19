
#include "G6.h"
#include "D7.h"
#include "S6.h"
#include "MatG6.h"
#include "MatS6.h"
#include "Reducer.h"
#include "Delone.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "D7Dist.h"
#include "CS6Dist_func.h"
#include "CS6Dist.h"
#include "NCDist.h"


#define ARMA_DONT_USE_BLAS
#define ARMA_DONT_USE_LAPACK
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctype.h>
#include <ctime>

using namespace std;

// *****************************************************************************
S6 makeprimredcell( std::string testlattice,
	double a, double b, double c, double alpha, double beta, double gamma, double extra )
{
    std::string latsym;
    char clatsym;
    G6 v6cell;
    G6 redprimcell;
    D7 d7redprimcell;
    S6 s6redprimcell;
    G6 dredprimcell;
    MatG6 mc;
    MatG6 m;
    MatG6 dm;
    G6 primcell;
    G6 recipcell;
    G6 reducedBase;
    G6 primredprobe;
    G6 dprimredprobe;
    double crootvol;
    LRL_Cell rawcell(a,b,c, alpha,beta,gamma);
    int ii;
    bool ret;
    if (testlattice.size()< 1) {
        latsym = "P";
    } else {
        latsym = testlattice.substr(0,1);
    }
    clatsym = latsym[0];
    switch (clatsym) {
        case 'P':
        case 'p':
        case 'A':
        case 'a':
        case 'B':
        case 'b':
        case 'C':
        case 'c':
        case 'I':
        case 'i':
        case 'F':
        case 'f':
        case 'R':
        case 'r':
        case 'H':
        case 'h':
            mc = rawcell.LatSymMatG6(latsym);
            primcell = mc*(G6(rawcell));
            break;
        case 'V':
        case 'v':
	    primcell[0] = a;
	    primcell[1] = b;
	    primcell[2] = c;
	    primcell[3] = alpha;
	    primcell[4] = beta;
	    primcell[5] = gamma;
            break;
        case 'D':
        case 'd':
           primcell[0] = a;
           primcell[1] = b;
           primcell[2] = c;
           primcell[3] = beta-b-c;
           primcell[4] = gamma-a-c;
           primcell[5] = extra-a-b;
           break;
        case 'S':
        case 's':
           primcell[3] = 2.*a;
           primcell[4] = 2.*b;
           primcell[5] = 2.*c;
           primcell[0] = -alpha-c-b;
           primcell[1] = -beta-c-a;
           primcell[2] = -gamma-b-a;
           break;
        default:
            std::cerr << "Unrecognized lattice symbol "<< testlattice<<" treated as P" << std::endl;
            latsym = "P";
            mc = rawcell.LatSymMatG6(latsym);
            primcell = mc*(G6(rawcell));
            break;
    }

    ret = Reducer::Reduce(primcell,m,redprimcell,0.0);
    s6redprimcell = redprimcell;
    ret = Delone::Reduce(primcell,dm,dredprimcell);
    d7redprimcell = D7(dredprimcell);
    //s6redprimcell = S6((d7redprimcell[4]-d7redprimcell[1]-d7redprimcell[2])/2.,
    //                   (d7redprimcell[5]-d7redprimcell[0]-d7redprimcell[2])/2.,
    //                   (d7redprimcell[6]-d7redprimcell[0]-d7redprimcell[1])/2.,
    //                   (d7redprimcell[4]-d7redprimcell[0]-d7redprimcell[3])/2.,
    //                   (d7redprimcell[5]-d7redprimcell[1]-d7redprimcell[3])/2.,
    //                   (d7redprimcell[6]-d7redprimcell[2]-d7redprimcell[3])/2.); 
    primredprobe =  LRL_Cell(redprimcell);
    dprimredprobe = LRL_Cell(dredprimcell);
    /*std::cerr << "Primitive Reduced Probe G6: " <<
    redprimcell[0]<<" "<<
    redprimcell[1]<<" "<<
    redprimcell[2]<<" "<<
    redprimcell[3]<<" "<<
    redprimcell[4]<<" "<<
    redprimcell[5] << std::endl;
    std::cerr << "Delaunay Primitive Reduced Probe G6: " <<
    dprimredprobe[0]<<" "<<
    dprimredprobe[1]<<" "<<
    dprimredprobe[2]<<" "<<
    dprimredprobe[3]<<" "<<
    dprimredprobe[4]<<" "<<
    dprimredprobe[5] << std::endl;
    std::cerr << "Delaunay Primitive Reduced Probe D7: " <<
    d7redprimcell[0]<<" "<<
    d7redprimcell[1]<<" "<<
    d7redprimcell[2]<<" "<<
    d7redprimcell[3]<<" "<<
    d7redprimcell[4]<<" "<<
    d7redprimcell[5]<<" "<<
    d7redprimcell[6] << std::endl;
    std::cerr << "Selling Primitive Reduced Probe S6: " <<
    s6redprimcell[0]<<" "<<
    s6redprimcell[1]<<" "<<
    s6redprimcell[2]<<" "<<
    s6redprimcell[3]<<" "<<
    s6redprimcell[4]<<" "<<
    s6redprimcell[5] << std::endl;
    std::cerr << "Volume :" << LRL_Cell(redprimcell).Volume() << std::endl;
    crootvol = pow(LRL_Cell(redprimcell).Volume(),1./3.);
    Reducer::Reduce(G6(LRL_Cell(redprimcell).Inverse()),m,reducedBase,0.0);
    recipcell = (LRL_Cell_Degrees(redprimcell).Inverse());    
    std::cerr << "Reciprocal of Primitive Probe G6: " <<
    recipcell[0]<<" "<<
    recipcell[1]<<" "<<
    recipcell[2]<<" "<<
    recipcell[3]<<" "<<
    recipcell[4]<<" "<<
    recipcell[5]<< std::endl;
    std::cerr << "Volume of Reciprocal Cell: " <<
    (LRL_Cell(redprimcell).Inverse()).Volume() << std::endl;
    std::cerr << "V7 linearized and scaled: "
    << primredprobe[0]*std::sqrt(6./7.)<<" "
    << primredprobe[1]*std::sqrt(6./7.)<<" "
    << primredprobe[2]*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[0])*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[1])*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[2])*std::sqrt(6./7.)<<" "
    << " "<<
    crootvol*std::sqrt(6./7.)<<std::endl;
    if (latsym[0] == 'V' || latsym[0] == 'v') {
        std::cerr << "raw G6 vector: " << primcell << std::endl;
    } else {
        std::cerr << "raw G6 vector: "<< dredprimcell <<std::endl;
    }
    std::cerr << std::endl;
    */
    return s6redprimcell;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool not_spaceA( const char c ) {
   return( c != ' ' );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool spaceA( const char c ) {
   return( c == ' ' );
}

/* Split a string on white space returninf a vector of strings */

const std::vector<std::string> SplitBetweenBlanksA( const std::string& s ) {
   std::vector<std::string> str;
   std::string::const_iterator i = s.begin( );

   while ( i != s.end( ) )
   {
      //look for the next non-blank
      i = std::find_if( i, s.end(), not_spaceA );
      const std::string::const_iterator i2 = std::find_if( i, s.end( ), spaceA );
      str.push_back( std::string( i, i2 ) );
      i = i2;
   }

   return( str );
}

int main(int argc, char ** argv) {

    std::string lat1, lat2;
    std::string arg0;
    std::string arg1;
    std::string line;
    std::vector<std::string> retlines;
    double a1,b1,c1,alpha1,beta1,gamma1,extra1;
    double a2,b2,c2,alpha2,beta2,gamma2,extra2;;
    S6 prim1, prim2;
    std::vector<G6> inputprims;
    double dprim1[6];
    double dprim2[6];
    double rawdist;
    size_t ii,jj,kk;
    char clatsym;
    int argoff;

    if (argc > 1) arg1 = std::string(argv[1]);
    if (arg1 == "--help" || arg1 == "-h") {
        std::cerr 
		<< "Usage: cs6dist_mat [--help|-h] print this message and exit" 
		<< std::endl;
        std::cerr 
		<< "       cs6dist_mat with no arguments, write distance matrix to cout" 
		<< std::endl;
        std::cerr 
		<< "                                      reading cells from cin" 
		<< std::endl;
        return -1;
    }

    while (std::getline(std::cin, line)) {
        if (line.size() == 0) break;
        retlines=SplitBetweenBlanksA(line);
        if (retlines.size() == 0) break;
        extra1 = 0.;
        lat1 = std::string(retlines[0]);
        clatsym= lat1.substr(0,1)[0];
        if (clatsym == 'D' || clatsym == 'd') {
        extra1 = atof(retlines[7].c_str());
        }
        a1 = atof(retlines[1].c_str());
        b1 = atof(retlines[2].c_str());
        c1 = atof(retlines[3].c_str());
        alpha1 = atof(retlines[4].c_str());
        beta1 = atof(retlines[5].c_str());
        gamma1 = atof(retlines[6].c_str());
        prim1 = makeprimredcell(lat1,a1,b1,c1,alpha1,beta1,gamma1,extra1);
        inputprims.push_back(prim1);
        /*ii = inputprims.size()-1;
        std::cerr << "ii: "<< ii << ": prim1: [" << prim1[0] <<", "<< prim1[1] << ", "<< prim1[2] << ", "
              << prim1[3] << ", " << prim1[4] << ", " << prim1[5] <<  "]" << std::endl;*/

    }

    auto start = std::clock();
    double elapsedTime1;
    double elapsedTime2;

    {
       std::vector<double>  dmat(inputprims.size()*inputprims.size(), 0.0);

       start = std::clock();
       for (ii = 0; ii < inputprims.size(); ii++) {
          for (jj = ii + 1; jj < inputprims.size(); jj++) {
             //rawdist = CS6Dist(dprim1, dprim2);   // lca
             rawdist = NCDist(inputprims[ii].data(), inputprims[jj].data());   // lca
             dmat[ii + jj * inputprims.size()] = dmat[jj + ii * inputprims.size()] = 0.1*std::sqrt(rawdist);
          }
       }
       elapsedTime1 = std::clock() - start;

       start = std::clock();
       for (ii = 0; ii < inputprims.size(); ii++) {
          for (jj = 0; jj < inputprims.size(); jj++) {
             std::cout << " " << dmat[ii + jj * inputprims.size()];
          }
          std::cout << std::endl;
       }
       elapsedTime2 = std::clock() - start;

    }

    std::cout << "elapsedTime1  " << elapsedTime1 << std::endl;
    std::cout << "elapsedTime2  " << elapsedTime2 << std::endl;
    return 0;
}
