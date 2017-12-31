
#ifndef GLITCH_H
#define GLITCH_H

#include "GlitchElement.h"

template<typename TVEC>
class Glitch
{
public:
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Glitch( void )
      : m_glitchData1( GlitchElement<TVEC>( ) )
      , m_glitchData2( GlitchElement<TVEC>( ) )
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Glitch(
      const GlitchElement<TVEC>& glitchData1,
      const GlitchElement<TVEC>& glitchData2
      )
      : m_glitchData1( glitchData1 )
      , m_glitchData2( glitchData2 )
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   ~Glitch( void )
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const GlitchElement<TVEC> GetGlitchElement1( void ) const
      /*-------------------------------------------------------------------------------------*/
   {
      return(m_glitchData1);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const GlitchElement<TVEC> GetGlitchElement2( void ) const
      /*-------------------------------------------------------------------------------------*/
   {
      return(m_glitchData2);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const double GetGlitchDistance1( void ) const
      /*-------------------------------------------------------------------------------------*/
   {
      return(m_glitchData1.GetDistance( ));
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const double GetGlitchDistance2( void ) const
      /*-------------------------------------------------------------------------------------*/
   {
      return(m_glitchData2.GetDistance( ));
   }


private:
   GlitchElement<TVEC> m_glitchData1;
   GlitchElement<TVEC> m_glitchData2;
};

#endif // GLITCH_H

