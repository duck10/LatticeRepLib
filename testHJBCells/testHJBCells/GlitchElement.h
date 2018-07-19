#if _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef GLITCHELEMENT_H
#define GLITCHELEMENT_H

#include <vector>

template<typename TVEC>
class GlitchElement
{
public:
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   GlitchElement( void )
      : m_glitchDistance( -1.0 )
      , m_glitchPosition( )
      , m_glichVector( )
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   GlitchElement(
      const double      glitchDistance1,
      const size_t      glitchPosition1,
      const TVEC& glichVector1
      )
      : m_glitchDistance( glitchDistance1 )
      , m_glitchPosition( glitchPosition1 )
      , m_glichVector( glichVector1 )
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   ~GlitchElement( void ) {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const double GetDistance( void ) const {
      return(m_glitchDistance);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const size_t GetPosition( void ) const {
      return(m_glitchPosition);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const TVEC GetVector( void ) const {
      return(m_glichVector);
   }

private:
   double     m_glitchDistance;
   size_t     m_glitchPosition;
   TVEC m_glichVector;
};

#endif
