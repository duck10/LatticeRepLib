
#include <RcppParallel.h>
using namespace RcppParallel;
#include <RcppArmadillo.h>

#include "Reducer.h"
#include "Cell.h"
#include "NCDist.h"
#include <stdlib.h>


//*****************************************************************************
G6 makeprimredprobe( std::string testlattice,
	double a, double b, double c, double alpha, double beta, double gamma )
{
    std::string latsym;
    char clatsym;
    G6 v6cell;
    G6 redprimcell;
    Mat66 mc;
    Mat66 m;
    G6 primcell;
    G6 recipcell;
    G6 reducedBase;
    G6 primredprobe;
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
        default:
            /* Rprintf("Unrecognized lattice symbol %s treated as P\n",testlattice.c_str()); */
            latsym = "P";
            mc = rawcell.LatSymMat66(latsym);
            primcell = mc*(rawcell.Cell2V6());
            break;
    }
    ret = Reducer::Reduce(primcell,m,redprimcell,0.0);
    primredprobe = Cell(redprimcell).CellWithDegrees();
    /* Rprintf("Primitive Reduced Probe Cell: [%g,%g,%g,%g,%g,%g]\n",
    primredprobe[0], primredprobe[1],primredprobe[2],primredprobe[3],primredprobe[4],primredprobe[5]);
    Rprintf("Volume : %g\n",Cell(redprimcell).Volume()); */
    crootvol = pow(Cell(redprimcell).Volume(),1./3.);
    Reducer::Reduce((Cell(redprimcell).Inverse()).Cell2V6(),m,reducedBase,0.0);
    recipcell = (Cell(redprimcell).Inverse()).CellWithDegrees();
    /* Rprintf("Reciprocal of Primitive Probe Cell: [%g,%g,%g,%g,%g,%g]\n",recipcell[0],recipcell[1],recipcell[2],recipcell[3],recipcell[4],recipcell[5]);
    Rprintf("Volume of Reciprocal Cell: %g\n", (Cell(redprimcell).Inverse()).Volume()); */
    if (latsym[0] == 'V' || latsym[0] == 'v') {
        /* Rprintf("raw G6 vector: [%g,%g,%g,%g,%g,%g]\n",primcell[0],primcell[1],primcell[2],primcell[3],primcell[4],primcell[5]); */
    } else {
        /* Rprintf("raw G6 vector: [%g,%g,%g,%g,%g,%g]\n",
        primredprobe[0]*primredprobe[0],
        primredprobe[1]*primredprobe[1],
        primredprobe[2]*primredprobe[2],
        2.*primredprobe[1]*primredprobe[2]*cos(primredprobe[3]*std::atan(1.0)/45.),
        2.*primredprobe[0]*primredprobe[2]*cos(primredprobe[4]*std::atan(1.0)/45.),
        2.*primredprobe[0]*primredprobe[1]*cos(primredprobe[5]*std::atan(1.0)/45.)); */
    }
    return primredprobe;
}

extern "C" SEXP rcpp_ncdist ( SEXP lat1_, SEXP a1_, SEXP b1_, SEXP c1_, 
                              SEXP alpha1_, SEXP beta1_, SEXP gamma1_,
			      SEXP lat2_, SEXP a2_, SEXP b2_, SEXP c2_, 
                              SEXP alpha2_, SEXP beta2_, SEXP gamma2_) {
    G6 prim1, prim2;
    double dprim1[6];
    double dprim2[6];
    size_t ii;

    std::string lat1 = Rcpp::as<std::string>(lat1_);
    double a1        = Rcpp::as<double>(a1_);
    double b1        = Rcpp::as<double>(b1_);
    double c1        = Rcpp::as<double>(c1_);
    double alpha1    = Rcpp::as<double>(alpha1_);
    double beta1     = Rcpp::as<double>(beta1_);
    double gamma1    = Rcpp::as<double>(gamma1_);
    std::string lat2 = Rcpp::as<std::string>(lat2_);
    double a2        = Rcpp::as<double>(a2_);
    double b2        = Rcpp::as<double>(b2_);
    double c2        = Rcpp::as<double>(c2_);
    double alpha2    = Rcpp::as<double>(alpha2_);
    double beta2     = Rcpp::as<double>(beta2_);
    double gamma2    = Rcpp::as<double>(gamma2_);
    prim1 = makeprimredprobe(lat1,a1,b1,c1,alpha1,beta1,gamma1);
    prim2 = makeprimredprobe(lat2,a2,b2,c2,alpha2,beta2,gamma2);
    Cell cell1 = Cell(prim1[0],prim1[1],prim1[2],prim1[3],prim1[4],prim1[5]);
    Cell cell2 = Cell(prim2[0],prim2[1],prim2[2],prim2[3],prim2[4],prim2[5]);
    G6 gv1 = G6(cell1.Cell2V6());
    G6 gv2 = G6(cell2.Cell2V6());
    for (ii=0; ii < 6; ii++) {
      dprim1[ii] = gv1[ii];
      dprim2[ii] = gv2[ii];
    }
    //std::cout << "dprim1: [" << dprim1[0] <<", "<< dprim1[1] << ", "<< dprim1[2] << ", "<< dprim1[3] << ", " << dprim1[4] << ", " << dprim1[1] <<"]" << std::endl;
    //std::cout << "dprim2: [" << dprim2[0] <<", "<< dprim2[1] << ", "<< dprim2[2] << ", "<< dprim2[3] << ", " << dprim2[4] << ", " << dprim2[1] <<"]" << std::endl;
    return Rcpp::wrap( 0.1*std::sqrt(NCDist(dprim1,dprim2)));
}


