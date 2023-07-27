/*
Derived from the code on
http://cboard.cprogramming.com/cplusplus-programming/117560-singleton-base-template-class-how-do-you-do.html

from "Sebastiani",  2001
*/


#ifndef COMSTSINGLETON_T_H
#define COMSTSINGLETON_T_H


template < typename T >
struct ConstSingleton_T
{
protected:

   ConstSingleton_T( void )
   {
      if (!instance_) instance_ = (T*)this;
   }

   virtual ~ConstSingleton_T( void )
   {
      instance_ = 0;
   }

public:
   static inline const T& instance( void )  // lca - why is this needed?
   {
      return *instance_;
   }

   static inline bool exists( void )
   {
      return instance_ != 0;
   }

   static T * instance_;

private:
   ConstSingleton_T operator=(const ConstSingleton_T& ) const; // do not implement  -- compiler will complain if you try to assign to the singleton
   ConstSingleton_T(const ConstSingleton_T& ); // do not implement  -- compiler will complain if you try to copy the singleton

};
 
template < typename T >
T* ConstSingleton_T< T >::instance_ = 0;

#endif
