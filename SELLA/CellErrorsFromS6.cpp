//LRL_Cell CellErrorsFromS6(const S6& s, const S6& sig) {
//   LRL_Cell cer;
//   double& da = cer[0];
//   double& db = cer[1];
//   double& dc = cer[2];
//   double& dal = cer[3];
//   double& dbe = cer[4];
//   double& dga = cer[5];
//
//   const double& ds1 = sig[0];
//   const double& ds2 = sig[1];
//   const double& ds3 = sig[2];
//   const double& ds4 = sig[3];
//   const double& ds5 = sig[4];
//   const double& ds6 = sig[5];
//
//   const double& s1 = s[0];
//   const double& s2 = s[1];
//   const double& s3 = s[2];
//   const double& s4 = s[3];
//   const double& s5 = s[4];
//   const double& s6 = s[5];
//
//   da = sqrt( 0.25 * (sq( -ds4 ) - sq( ds3 ) - sq( ds2 )) / (-s4 - s3 - s2) );
//   db = sqrt( 0.25 * (sq( -ds5 ) - sq( ds3 ) - sq( ds1 )) / (-s5 - s3 - s1) );
//   dc = sqrt( 0.25 * (sq( -ds6 ) - sq( ds2 ) - sq( ds1 )) / (-s6 - s2 - s1) );
//
//   const double aas = -s4 - s3 - s2; // this is a^2
//   const double bas = -s5 - s3 - s1;
//   const double cas = -s6 - s2 - s1;
//   const double sqrta = sqrt( aas ); // this is just a
//   const double sqrtb = sqrt( bas ); // this is just b
//   const double sqrtc = sqrt( cas ); // this is just c
//
//   double dsqrta = -0.5 / (sqrta * sqrta * sqrta) * sqrt(sq( ds4 ) + sq( ds3 ) + sq( ds2 ));
//   double dsqrtb = -0.5 / (sqrtb * sqrtb * sqrtb) * sqrt(sq( ds5 ) + sq( ds3 ) + sq( ds1 ));
//   double dsqrtc = -0.5 / (sqrtc * sqrtc * sqrtc) * sqrt(sq( ds6 ) + sq( ds2 ) + sq( ds1 ));
//
//   double tds1[6] = {
//   0,
//   -ds1 / (2 * sqrt( (-s5) - s3 - s1 )),
//
//   -ds1 / (2 * sqrt( (-s6) - s2 - s1 )),
//
//   -(ds1 * (1 / (sqrt( (-s5) - s3 - s1 ) * sqrt( (-s6) - s2 - s1 ))
//      + s1 / (2 * pow( ((-s5) - s3 - s1) , 1.5 ) * sqrt( (-s6) - s2 - s1 ))
//      + s1 / (2 * sqrt( (-s5) - s3 - s1 ) * pow( ((-s6) - s2 - s1) , 1.5 ))))
//   / sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) ),
//
//   -(ds1 * s2) / (2 * sqrt( (-s4) - s3 - s2 )
//      * sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
//      * pow( ((-s6) - s2 - s1) , 1.5 )),
//
//
//   -(ds1 * s3) / (2 * sqrt( (-s4) - s3 - s2 )
//      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
//      * ((-s5) - s3 - s1) , 1.5) };
//
//
//
//   // ds2 ---------------------------------------------------------------------
//
//double tds2[6] = { 
//   -ds2 / (2 * sqrt( (-s4) - s3 - s2 )),
//
//   0,
//
//   -ds2 / (2 * sqrt( (-s6) - s2 - s1 )),
//
//   -(ds2 * s1) / (2 * sqrt( (-s5) - s3 - s1 )
//      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
//      * pow( ((-s6) - s2 - s1) , 1.5 )),
//
//   -(ds2 * (1 / (sqrt( (-s4) - s3 - s2 ) * sqrt( (-s6) - s2 - s1 ))
//      + s2 / (2 * pow( ((-s4) - s3 - s2) , 1.5 ) * sqrt( (-s6) - s2 - s1 ))
//      + s2 / (2 * sqrt( (-s4) - s3 - s2 ) * pow( ((-s6) - s2 - s1) , 1.5 ))))
//    /sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) ),
//
//   -(ds2 * s3) / (2 * pow( ((-s4) - s3 - s2) , 1.5 )
//      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
//      * sqrt( (-s5) - s3 - s1 )) };
//// ds3 ---------------------------------------------------------------------
//
//double tds3[6] = {
//   -ds3 / (2 * sqrt( (-s4) - s3 - s2 )), 
//   -ds3 / (2 * sqrt( (-s5) - s3 - s1 )), 
//   0,
//   -(ds3 * s1) / (2 * pow( ((-s5) - s3 - s1) , 1.5 )
//      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
//      * sqrt( (-s6) - s2 - s1 )),
//
//      -(ds3 * s2) / (2 * pow( ((-s4) - s3 - s2) , 1.5 )
//   * sqrt( 1 - s2 * s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
//   * sqrt( (-s6) - s2 - s1 )),
//
//   -(ds3 * (1 / (sqrt( (-s4) - s3 - s2 ) * sqrt( (-s5) - s3 - s1 ))
//      + s3 / (2 * pow( ((-s4) - s3 - s2) , 1.5 ) * sqrt( (-s5) - s3 - s1 ))
//      + s3 / (2 * sqrt( (-s4) - s3 - s2 ) * pow( ((-s5) - s3 - s1) , 1.5 ))))
//      / sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) ) };
//
//      // ds4 ---------------------------------------------------------------------
//
//double tds4[6] = {
// -ds4 / (2 * sqrt( (-s4) - s3 - s2 )), 
// 0, 
// 0, 
// 0,
//
//   -(ds4 * s2) / (2 * pow( ((-s4) - s3 - s2) , 1.5)
//      * sqrt( 1 - s2 *s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
//      * sqrt( (-s6) - s2 - s1 )),
//
//   -(ds4 * s3) / (2 * pow( ((-s4) - s3 - s2) , 1.5)
//      * sqrt( 1 - s3 *s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
//      * sqrt( (-s5) - s3 - s1 ))};
//
//      // ds5 ---------------------------------------------------------------------
//
//double tds5[6] = {
//   0, 
//   -ds5 / (2 * sqrt( (-s5) - s3 - s1 )), 
//   0,
//   -(ds5 * s1) / (2 * pow( ((-s5) - s3 - s1) , 1.5)
//      * sqrt( 1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
//      * sqrt( (-s6) - s2 - s1 )),
//   0,
//   -(ds5 * s3) / (2 * sqrt( (-s4) - s3 - s2 )
//      * sqrt( 1 - s3 * s3 / (((-s4) - s3 - s2) * ((-s5) - s3 - s1)) )
//      * pow( ((-s5) - s3 - s1) , 1.5)) };
//
//      // ds6 ---------------------------------------------------------------------
//
//double tds6[6] = {
//   0, 
//   0, 
//   -ds6 / (2 * sqrt( (-s6) - s2 - s1 )),
//   -(ds6 * s1) / (2 * sqrt( (-s5) - s3 - s1 )
//      * sqrt( 1 - s1 *s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)) )
//      * pow( ((-s6) - s2 - s1) , 1.5)),
//   -(ds6 * s2) / (2 * sqrt( (-s4) - s3 - s2 )
//      * sqrt( 1 - s2 *s2 / (((-s4) - s3 - s2) * ((-s6) - s2 - s1)) )
//      * pow( ((-s6) - s2 - s1), 1.5 )), 
//   0};
//
//for (size_t i = 0; i < 6; ++i) {
//   cer[i] = sqrt( sq( tds1[i] ) + sq( tds2[i] ) + sq( tds3[i] ) + sq( tds4[i] ) + +sq( tds5[i] ) + sq( tds6[i] ) );
//}
//
//for (size_t i = 0; i < 6; ++i) {
//   std::cout << ( tds1[i] ) << "  " << ( tds2[i] ) << "  " << ( tds3[i] ) << "  " << ( tds4[i] ) << "  " <<  "  " << ( tds5[i] ) << "  " << ( tds6[i] ) << std::endl;;
//}
//
//
//
//
//   {
//      double denom = (2 * sqrt(1 - s1 * s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1))) * pow(((-s5) - s3 - s1) * ((-s6) - s2 - s1),1.5     ) );
//      double t1 = - (1 / sqrt(((-s5) - s3 - s1) * ((-s6) - s2 - s1))
//         - (s1 * (s6 + s5 + s3 + s2 + 2 * s1)) / (2 * pow(((-s5) - s3 - s1) * ((-s6) - s2 - s1),1.5       ) )) / sqrt(1 - s1*s1 / (((-s5) - s3 - s1) * ((-s6) - s2 - s1)));
//      double t2 = (s1 * (s5 + s3 + s1)) / denom;
//      double t3 = (s1 * (s6 + s2 + s1)) / denom;
//      double t4 = 0;
//      double t5 = (s1 * (s6 + s2 + s1)) / denom;
//      double t6 = (s1 * (s5 + s3 + s1)) / denom;
//      double deral = sqrt(sq(t1 * ds1) + sq(t2 * ds2) + sq(t3 * ds3) + sq(t4 * ds4) + sq(t5 * ds5) + sq(t6 * ds6));
//
//      double test = deral * 1.0 / sqrt( 6.0 );
//
//      std::cout << "deral " << deral << std::endl;
//      const int i19191 = 19191;
//   }
//
//   return cer;
//}
//
//S6 S6ErrorsFromCell( const LRL_Cell& cell, const LRL_Cell& errorsInRadians ) {
//   S6 s6errors;
//
//   const double& a = cell[0];
//   const double& b = cell[1];
//   const double& c = cell[2];
//   const double& al = cell[3]; // should be radians !!!!!
//   const double& be = cell[4];
//   const double& ga = cell[5];
//
//const double& da = errorsInRadians[0];
//const double& db = errorsInRadians[1];
//const double& dc = errorsInRadians[2];
//const double& dal = errorsInRadians[3];
//const double& dbe = errorsInRadians[4];
//const double& dga = errorsInRadians[5];
//
//const double cosal = cos(al);
//const double cosbe = cos(be);
//const double cosga = cos(ga);
//const double sinal = sin(al);
//const double sinbe = sin(be);
//const double singa = sin(ga);
//
//std::cout << std::endl;
//std::cout << "   db * c * cosal " << db * c * cosal << std::endl;
//std::cout << "   da * c * cosbe " << da * c * cosbe << std::endl;
//std::cout << "   da * b * cosga " << da * b * cosga << std::endl;
//std::cout << "   dc * b * cosal  " << dc * b * cosal << std::endl;
//std::cout << "   dc * a * cosbe  " << dc * a * cosbe << std::endl;
//std::cout << "   db * a * cosga  " << db * a * cosga << std::endl;
//std::cout << " -dal * b * c * sinal " << -dal * b * c * sinal << std::endl;
//std::cout << " -dbe * a * c * sinbe " << -dbe * a * c * sinbe << std::endl;
//std::cout << " -dga * a * b * singa " << -dga * a * b * singa << std::endl;
//std::cout << std::endl;
//
//s6errors[0] = sqrt(sq(db * c * cosal) + sq(dc * b * cosal) + sq(-dal * b * c * sinal));
//s6errors[1] = sqrt(sq(da * c * cosbe) + sq(dc * a * cosbe) + sq(-dbe * a * c * sinbe));
//s6errors[2] = sqrt(sq(da * b * cosga) + sq(db * a * cosga) + sq(-dga * a * b * singa));
//
//s6errors[3] = sqrt(sq(-2.0 * a * da) + sq(-s6errors[2]) + sq(-s6errors[1]));
//s6errors[4] = sqrt(sq(-2.0 * b * db) + sq(-s6errors[2]) + sq(-s6errors[0]));
//s6errors[5] = sqrt(sq(-2.0 * c * dc) + sq(-s6errors[0]) + sq(-s6errors[1]));
//
//s6errors[0] = maxNC(abs(db * c * cosal), abs(dc * b * cosal), abs(-dal * b * c * sinal));
//s6errors[1] = maxNC(abs(da * c * cosbe), abs(dc * a * cosbe), abs(-dbe * a * c * sinbe));
//s6errors[2] = maxNC(abs(da * b * cosga), abs(db * a * cosga), abs(-dga * a * b * singa));
//
//s6errors[3] = maxNC(abs(-2.0 * a * da), abs(-s6errors[2]), abs(-s6errors[1]));
//s6errors[4] = maxNC(abs(-2.0 * b * db), abs(-s6errors[2]), abs(-s6errors[0]));
//s6errors[5] = maxNC(abs(-2.0 * c * dc), abs(-s6errors[0]), abs(-s6errors[1]));
//
//return s6errors / sqrt(6.0);
//}
//
//template<typename T>
//std::string InputLattice(const size_t n, const std::string& lattice, const T& t, const LRL_Cell& c) {
//   LatticeConverter converter;
//   MatS6 ms;
//   MatG6 mg;
//   std::ostringstream o;
//   o << "\tOriginal Cell " << n << std::endl;
//   o << "input " << lattice << " " << LRL_Cell_Degrees(t) << std::endl;
//   const S6 s6Red = converter.SellingReduceCell(lattice, S6(t), ms);
//   o << "errors " << LRL_Cell_Degrees(c) << std::endl;
//   return o.str();
//}


static double sq(const double d) { return d * d; }

//std::vector<S6> CreateS6Errors(const std::vector< S6>& vs) {
//   std::vector<S6> out;
//   for (size_t i = 0; i < vs.size(); ++i) {
//      out.push_back(0.1 * vs[i]);
//   }
//   return out;
//}
//
//std::vector<LRL_Cell> CreateE3Errors(const std::vector<LRL_Cell>& vc) {
//   std::vector<LRL_Cell> out;
//   LRL_Cell cellErrors;
//   for (size_t i = 0; i < vc.size(); ++i) {
//      cellErrors = 0.01 * vc[i];
//      cellErrors[3] = 0.01 * vc[i][3];
//      cellErrors[4] = 0.01 * vc[i][4];
//      cellErrors[5] = 0.01 * vc[i][5];
//      out.push_back(cellErrors);
//   }
//   return out;
//}
