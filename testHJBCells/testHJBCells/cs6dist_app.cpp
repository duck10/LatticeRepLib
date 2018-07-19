
#include "G6.h"
#include "D7.h"
#include "S6.h"
#include "Reducer.h"
#include "Delone.h"
#include "Cell.h"
#include "D7Dist.h"
#include "CS6Dist_func.h"


#define ARMA_DONT_USE_BLAS
#define ARMA_DONT_USE_LAPACK
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctype.h>

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
    Mat66 mc;
    Mat66 m;
    Mat66 dm;
    G6 primcell;
    G6 recipcell;
    G6 reducedBase;
    G6 primredprobe;
    G6 dprimredprobe;
    double crootvol;
    Cell rawcell(a,b,c, alpha,beta,gamma);
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
            mc = rawcell.LatSymMat66(latsym);
            primcell = mc*(rawcell.Cell2V6());
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
            mc = rawcell.LatSymMat66(latsym);
            primcell = mc*(rawcell.Cell2V6());
            break;
    }
    ret = Reducer::Reduce(primcell,m,redprimcell,0.0);
    ret = Delone::Reduce(primcell,dm,dredprimcell,0.0);
    d7redprimcell = D7(dredprimcell);
    s6redprimcell = S6((d7redprimcell[4]-d7redprimcell[1]-d7redprimcell[2])/2.,
                       (d7redprimcell[5]-d7redprimcell[0]-d7redprimcell[2])/2.,
                       (d7redprimcell[6]-d7redprimcell[0]-d7redprimcell[1])/2.,
                       (d7redprimcell[4]-d7redprimcell[0]-d7redprimcell[3])/2.,
                       (d7redprimcell[5]-d7redprimcell[1]-d7redprimcell[3])/2.,
                       (d7redprimcell[6]-d7redprimcell[2]-d7redprimcell[3])/2.); 
    primredprobe = Cell(redprimcell).CellWithDegrees();
    dprimredprobe = Cell(dredprimcell).CellWithDegrees();
    std::cout << "Primitive Reduced Probe Cell: " <<
    primredprobe[0]<<" "<<
    primredprobe[1]<<" "<<
    primredprobe[2]<<" "<<
    primredprobe[3]<<" "<<
    primredprobe[4]<<" "<<
    primredprobe[5] << std::endl;
    std::cout << "Delaunay Primitive Reduced Probe Cell: " <<
    dprimredprobe[0]<<" "<<
    dprimredprobe[1]<<" "<<
    dprimredprobe[2]<<" "<<
    dprimredprobe[3]<<" "<<
    dprimredprobe[4]<<" "<<
    dprimredprobe[5] << std::endl;
    std::cout << "Delaunay Primitive Reduced Probe D7 Cell: " <<
    d7redprimcell[0]<<" "<<
    d7redprimcell[1]<<" "<<
    d7redprimcell[2]<<" "<<
    d7redprimcell[3]<<" "<<
    d7redprimcell[4]<<" "<<
    d7redprimcell[5]<<" "<<
    d7redprimcell[6] << std::endl;
    std::cout << "Selling Primitive Reduced Probe S6 Cell: " <<
    s6redprimcell[0]<<" "<<
    s6redprimcell[1]<<" "<<
    s6redprimcell[2]<<" "<<
    s6redprimcell[3]<<" "<<
    s6redprimcell[4]<<" "<<
    s6redprimcell[5] << std::endl;
    std::cout << "Volume :" << Cell(redprimcell).Volume() << std::endl;
    crootvol = pow(Cell(redprimcell).Volume(),1./3.);
    Reducer::Reduce((Cell(redprimcell).Inverse()).Cell2V6(),m,reducedBase,0.0);
    recipcell = (Cell(redprimcell).Inverse()).CellWithDegrees();    
    std::cout << "Reciprocal of Primitive Probe Cell: " <<
    recipcell[0]<<" "<<
    recipcell[1]<<" "<<
    recipcell[2]<<" "<<
    recipcell[3]<<" "<<
    recipcell[4]<<" "<<
    recipcell[5]<< std::endl;
    std::cout << "Volume of Reciprocal Cell: " <<
    (Cell(redprimcell).Inverse()).Volume() << std::endl;
    std::cout << "V7 linearized and scaled: "
    << primredprobe[0]*std::sqrt(6./7.)<<" "
    << primredprobe[1]*std::sqrt(6./7.)<<" "
    << primredprobe[2]*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[0])*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[1])*std::sqrt(6./7.)<<" "
    << std::sqrt(1./reducedBase[2])*std::sqrt(6./7.)<<" "
    << " "<<
    crootvol*std::sqrt(6./7.)<<std::endl;
    if (latsym[0] == 'V' || latsym[0] == 'v') {
        std::cout << "raw G6 vector: "
        << primcell[0]<<" "
        << primcell[1]<<" "
        << primcell[2]<<" "
        << primcell[3]<<" "
        << primcell[4]<<" "
        << primcell[5]<<std::endl;
    } else {
        std::cout << "raw G6 vector: "
        << dprimredprobe[0]*dprimredprobe[0]<<" "
        << dprimredprobe[1]*dprimredprobe[1]<<" "
        << dprimredprobe[2]*dprimredprobe[2]<<" "
        << 2.*dprimredprobe[1]*dprimredprobe[2]*cos(dprimredprobe[3]*std::atan(1.0)/45.)<<" "
        << 2.*dprimredprobe[0]*dprimredprobe[2]*cos(dprimredprobe[4]*std::atan(1.0)/45.)<<" "
        << 2.*dprimredprobe[0]*dprimredprobe[1]*cos(dprimredprobe[5]*std::atan(1.0)/45.)<<std::endl;
    }
    std::cout << std::endl;
    return s6redprimcell;
}


int main(int argc, char ** argv) {

    std::string lat1, lat2;
    double a1,b1,c1,alpha1,beta1,gamma1,extra1;
    double a2,b2,c2,alpha2,beta2,gamma2,extra2;;
    S6 prim1, prim2;
    double dprim1[6];
    double dprim2[6];
    double rawdist;
    size_t ii;
    char clatsym;
    int argoff;

         
    if (argc < 15) {
        std::cerr 
		<< "Usage: cs6dist_app lat1 a1 b1 c1 alpha1 beta1 gamma1 lat2 a2 b2 c2 alpha2 beta2 gamma2" 
		<< std::endl;
        return -1;
    }
    clatsym=((std::string(argv[8])).substr(0,1))[0];
    argoff = 1;
    if (isalpha(clatsym)) argoff = 0;
    extra1 = 0.;
    extra2 = 0.;
    lat1 = std::string(argv[1]);
    clatsym= lat1.substr(0,1)[0];
    /* std::cerr <<  "clatsym, argoff " << clatsym << " " << argoff <<  std::endl; */
    if (clatsym == 'D' || clatsym == 'd') {
        argoff = 1;
        if (argc < 16) {
            std::cerr 
                << "Usage: cs6dist_app lat1 a1 b1 c1 alpha1 beta1 gamma1 lat2 a2 b2 c2 alpha2 beta2 gamma2" 
                << std::endl;
            return -1;
        }
        extra1 = atof(argv[8]);
    }
    lat2 = std::string(argv[8+argoff]);
    clatsym= lat2.substr(0,2)[0];
    if (clatsym == 'D' || clatsym == 'd') {
        if (argc < 16+argoff){
            std::cerr 
                << "Usage: cs6dist_app lat1 a1 b1 c1 alpha1 beta1 gamma1 lat2 a2 b2 c2 alpha2 beta2 gamma2" 
                << std::endl;
            return -1;
        }
        extra2 = atof(argv[15+argoff]);
    }

    a1 = atof(argv[2]);
    a2 = atof(argv[9+argoff]);
    b1 = atof(argv[3]);
    b2 = atof(argv[10+argoff]);
    c1 = atof(argv[4]);
    c2 = atof(argv[11+argoff]);
    alpha1 = atof(argv[5]);
    alpha2 = atof(argv[12+argoff]);
    beta1 = atof(argv[6]);
    beta2 = atof(argv[13+argoff]);
    gamma1 = atof(argv[7]);
    gamma2 = atof(argv[14+argoff]);
    
    prim1 = makeprimredcell(lat1,a1,b1,c1,alpha1,beta1,gamma1,extra1);
    prim2 = makeprimredcell(lat2,a2,b2,c2,alpha2,beta2,gamma2,extra2);
    for (ii=0; ii < 6; ii++) {
      dprim1[ii] = prim1[ii];
      dprim2[ii] = prim2[ii];
    }
    std::cout << "dprim1: [" << dprim1[0] <<", "<< dprim1[1] << ", "<< dprim1[2] << ", "
              << dprim1[3] << ", " << dprim1[4] << ", " << dprim1[5] <<  "]" << std::endl;
    std::cout << "dprim2: [" << dprim2[0] <<", "<< dprim2[1] << ", "<< dprim2[2] << ", "
              << dprim2[3] << ", " << dprim2[4] << ", " << dprim2[5] <<  "]" << std::endl;
    rawdist = CS6Dist_func(dprim1,dprim2);
    std::cout << "rawdist: "<< rawdist << std::endl;
    std::cout << 0.1*std::sqrt(rawdist) << std::endl;
    return 0;
}
