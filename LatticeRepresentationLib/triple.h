/*
 *  triple.h
 *  
 *
 *  Created by Herbert J. Bernstein on 10/26/10.
 *  Copyright 2010 Herbert J. Bernstein
 *
 */
 
/**********************************************************************
 *                                                                    *
 * YOU MAY REDISTRIBUTE THE CVector API UNDER THE TERMS OF THE LGPL   *
 *                                                                    *
 **********************************************************************/

/************************* LGPL NOTICES *******************************
 *                                                                    *
 * This library is free software; you can redistribute it and/or      *
 * modify it under the terms of the GNU Lesser General Public         *
 * License as published by the Free Software Foundation; either       *
 * version 2.1 of the License, or (at your option) any later version. *
 *                                                                    *
 * This library is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
 * Lesser General Public License for more details.                    *
 *                                                                    *
 * You should have received a copy of the GNU Lesser General Public   *
 * License along with this library; if not, write to the Free         *
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
 * MA  02110-1301  USA                                                *
 *                                                                    *
 **********************************************************************/

/************************* PAIR.H NOTICE ******************************
 * triple.h was derived from pair.h                                   *
 *  which was subject to the following notice:                        *
 *                                                                    *
 * Copyright (c) 1994                                                 *
 * Hewlett-Packard Company                                            *
 *                                                                    *
 * Permission to use, copy, modify, distribute and sell this software *
 * and its documentation for any purpose is hereby granted without    *
 * fee, provided that the above copyright notice appear in all copies *
 * and that both that copyright notice and this permission notice     *
 * appear in supporting documentation.  Hewlett-Packard Company makes *
 * no representations about the suitability of this software for any  *
 * purpose.  It is provided "as is" without express or implied        *
 * warranty.                                                          *
 *                                                                    *
 **********************************************************************/

#ifndef TRIPLE_H
#define TRIPLE_H

#include <cstdlib>
#include <iostream>

template <typename TR1, typename TR2, typename TR3>
class triple {
private:
    TR1 m_first;
    TR2 m_second;
    TR3 m_third;
public:

   friend std::ostream& operator<< ( std::ostream& o, const triple<TR1, TR2, TR3>& v ) {
      o << v.first  << std::endl;
      o << v.second << std::endl;
      o << v.third  << std::endl;
      return o;
   }

    triple() : m_first(TR1()), m_second(TR2()), m_third(TR3()), first(m_first), second(m_second), third(m_third) {}
    triple(const TR1& ifirst, const TR2& isecond, const TR3& ithird ) : m_first(ifirst), m_second(isecond), m_third(ithird), first(m_first), second(m_second), third(m_third) {}
    inline TR1 GetFirst ( void ) const  { return m_first;  }  
    inline TR2 GetSecond( void ) const { return m_second; }  
    inline TR3 GetThird ( void ) const  { return m_third;  }  

inline bool operator==(const triple<TR1, TR2, TR3>& rhs) { 
    return m_first == rhs.GetFirst() && m_second == rhs.m_second && m_third == rhs.m_third; 
}

inline bool operator<(const triple<TR1, TR2, TR3>& rhs) { 
    return m_first < rhs.GetFirst() 
      || (m_first == rhs.GetFirst() && m_second < rhs.GetSecond)
      || (m_first == rhs.GetFirst() && m_second < rhs.GetSecond && m_third < rhs.GetThird); 
}

inline triple& operator=( const triple& tr )
{
   m_first  = tr.m_first;
   m_second = tr.m_second;
   m_third  = tr.m_third;
   return ( *this );
}

   // copy constructor
   triple( const triple& tr )
      : m_first (tr.m_first)
      , m_second(tr.m_second)
      , m_third (tr.m_third)
      , first ( m_first )
      , second( m_second )
      , third ( m_third )
   {
   }

public:
   TR1 first;
   TR2 second;
   TR3 third;

};

template <class TR1, class TR2, class TR3>
inline triple<TR1, TR2, TR3> make_triple(const TR1& first, const TR2& second, const TR3& third) {
    return triple<TR1, TR2, TR3>(first, second, third);
}


#endif
