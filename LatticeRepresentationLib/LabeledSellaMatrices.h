#ifndef LABELEDSELLAMATRICES_H
#define LABELEDSELLAMATRICES_H

#include <string>
#include <vector>

#include "MatS6.h"

class LabeledSellaMatrices {
public:
   LabeledSellaMatrices()
   {}
   LabeledSellaMatrices(const std::string& label, const std::vector<MatS6>& vm) : m_label(label), m_vector(vm)
   {}



   static std::vector<LabeledSellaMatrices> CreateAllPerps();
   static std::vector<LabeledSellaMatrices> CreateAllPrjs();
   static std::vector<LabeledSellaMatrices> CreateAllToCanon( );
   std::string GetLabel() const { return m_label; }

   std::vector<MatS6>  GetMatrices() const { return m_vector; }

   size_t size() const { return m_vector.size(); }

   static const std::vector<LabeledSellaMatrices> projectors;
   static const std::vector<LabeledSellaMatrices> perps;
   static const std::vector<LabeledSellaMatrices> toCanons;

   std::vector<LabeledSellaMatrices> GetPerps( ) const { return perps; }
   std::vector<LabeledSellaMatrices> GetPrjs( ) const { return projectors; }
   std::vector<LabeledSellaMatrices> GetToCanons( ) const { return toCanons; }

   static LabeledSellaMatrices CreatePrjs_A1();
   static LabeledSellaMatrices CreatePrjs_A2();
   static LabeledSellaMatrices CreatePrjs_A3();
   static LabeledSellaMatrices CreatePrjs_H4();
   static LabeledSellaMatrices CreatePrjs_C1();
   static LabeledSellaMatrices CreatePrjs_C3();
   static LabeledSellaMatrices CreatePrjs_C5();
   static LabeledSellaMatrices CreatePrjs_M1A();
   static LabeledSellaMatrices CreatePrjs_M1B();
   static LabeledSellaMatrices CreatePrjs_M2A();
   static LabeledSellaMatrices CreatePrjs_M2B();
   static LabeledSellaMatrices CreatePrjs_M3();
   static LabeledSellaMatrices CreatePrjs_M4();
   static LabeledSellaMatrices CreatePrjs_O1A();
   static LabeledSellaMatrices CreatePrjs_O1B();
   static LabeledSellaMatrices CreatePrjs_O2();
   static LabeledSellaMatrices CreatePrjs_O3();
   static LabeledSellaMatrices CreatePrjs_O4();
   static LabeledSellaMatrices CreatePrjs_O4B();
   static LabeledSellaMatrices CreatePrjs_O4C();
   static LabeledSellaMatrices CreatePrjs_O5();
   static LabeledSellaMatrices CreatePrjs_T1();
   static LabeledSellaMatrices CreatePrjs_T2();
   static LabeledSellaMatrices CreatePrjs_T5();
   static LabeledSellaMatrices CreatePrjs_R1();
   static LabeledSellaMatrices CreatePrjs_R3();

   static LabeledSellaMatrices CreatePerps_A1();
   static LabeledSellaMatrices CreatePerps_A2();
   static LabeledSellaMatrices CreatePerps_A3();
   static LabeledSellaMatrices CreatePerps_H4();
   static LabeledSellaMatrices CreatePerps_C1();
   static LabeledSellaMatrices CreatePerps_C3();
   static LabeledSellaMatrices CreatePerps_C5();
   static LabeledSellaMatrices CreatePerps_M1A();
   static LabeledSellaMatrices CreatePerps_M1B();
   static LabeledSellaMatrices CreatePerps_M2A();
   static LabeledSellaMatrices CreatePerps_M2B();
   static LabeledSellaMatrices CreatePerps_M3();
   static LabeledSellaMatrices CreatePerps_M4();
   static LabeledSellaMatrices CreatePerps_O1A();
   static LabeledSellaMatrices CreatePerps_O1B();
   static LabeledSellaMatrices CreatePerps_O2();
   static LabeledSellaMatrices CreatePerps_O3();
   static LabeledSellaMatrices CreatePerps_O4();
   static LabeledSellaMatrices CreatePerps_O4B();
   static LabeledSellaMatrices CreatePerps_O4C();
   static LabeledSellaMatrices CreatePerps_O5();
   static LabeledSellaMatrices CreatePerps_T1();
   static LabeledSellaMatrices CreatePerps_T2();
   static LabeledSellaMatrices CreatePerps_T5();
   static LabeledSellaMatrices CreatePerps_R1();
   static LabeledSellaMatrices CreatePerps_R3();

   static LabeledSellaMatrices CreateToCanon_A1( );
   static LabeledSellaMatrices CreateToCanon_A2( );
   static LabeledSellaMatrices CreateToCanon_A3( );
   static LabeledSellaMatrices CreateToCanon_C1( );
   static LabeledSellaMatrices CreateToCanon_C3( );
   static LabeledSellaMatrices CreateToCanon_C5( );
   static LabeledSellaMatrices CreateToCanon_H4( );
   static LabeledSellaMatrices CreateToCanon_M1A( );
   static LabeledSellaMatrices CreateToCanon_M1B( );
   static LabeledSellaMatrices CreateToCanon_M2A( );
   static LabeledSellaMatrices CreateToCanon_M2B( );
   static LabeledSellaMatrices CreateToCanon_M3( );
   static LabeledSellaMatrices CreateToCanon_M4( );
   static LabeledSellaMatrices CreateToCanon_O1A( );
   static LabeledSellaMatrices CreateToCanon_O1B( );
   static LabeledSellaMatrices CreateToCanon_O2( );
   static LabeledSellaMatrices CreateToCanon_O3( );
   static LabeledSellaMatrices CreateToCanon_O4();
   static LabeledSellaMatrices CreateToCanon_O4B();
   static LabeledSellaMatrices CreateToCanon_O4C();
   static LabeledSellaMatrices CreateToCanon_O5( );
   static LabeledSellaMatrices CreateToCanon_R1( );
   static LabeledSellaMatrices CreateToCanon_R3( );
   static LabeledSellaMatrices CreateToCanon_T1( );
   static LabeledSellaMatrices CreateToCanon_T2( );
   static LabeledSellaMatrices CreateToCanon_T5( );
private:

   std::string m_label;
   std::vector<MatS6> m_vector;
};

#endif  // LABELEDSELLAMATRICES_H