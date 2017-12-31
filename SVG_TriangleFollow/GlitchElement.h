
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
      const unsigned long      glitchPosition1,
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
   const unsigned long GetPosition( void ) const {
      return(m_glitchPosition);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const TVEC GetVector( void ) const {
      return(m_glichVector);
   }

private:
   double     m_glitchDistance;
   unsigned long     m_glitchPosition;
   TVEC m_glichVector;
};

#endif
