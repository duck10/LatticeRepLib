#pragma once

#include <memory>
#include <string>

class AnotherPath {

   template<typename T>
   AnotherPath( const T& t) {}

private:

   struct HolderBase {
      virtual ~HolderBase() { }
      virtual std::string toString() const = 0;
      virtual std::unique_ptr<HolderBase> clone() const = 0;
   };

   template<typename T>
   struct Holder : public HolderBase {
      Holder(const T& obj)
         : obj_(obj)
      {
      }

      std::string toString() const override
      {
         return obj_.toString();
      }

      std::unique_ptr<HolderBase> clone() const override
      {
         return std::make_unique<Holder<T> >(obj_);
      }

      T obj_;
   };

   std::unique_ptr<HolderBase> inner_;
};


   std::unique_ptr<HolderBase> m_inner_;


};