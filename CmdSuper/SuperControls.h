#ifndef CMDSUPER_CONTROLS_H
#define CMDSUPER_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

#include <iostream>
#include <set>
#include <sstream>
#include <string>

// Controls for CmdSuper: generates supercells using transformation matrices
// from Santoro & Mighell, Acta Cryst. A28, 284-287 (1972).

class SuperControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const SuperControls& ctrl) {
      os << ctrl.getState();
      return os;
   }

   SuperControls() {
      // ORDER and MATRIXORDER are synonyms: restrict output to a single order.
      // If not set, all three orders (2, 3, 4) are computed.
      InputHandler::registerHandler("ORDER", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setOrder(std::stoi(value));
         });

      InputHandler::registerHandler("MATRIXORDER", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setOrder(std::stoi(value));
         });

      // SHOW prints the current control state and continues.
      InputHandler::registerHandler("SHOW", 0.35,
         [this](BaseControlVariables&, const std::string&) {
            std::cout << getState();
         });
   }

   // Returns the active set of orders to compute.
   // If the user specified a single order, only that order is returned;
   // otherwise all three default orders {2, 3, 4} are returned.
   std::vector<int> getOrders() const {
      if (specificOrder > 0)
         return { specificOrder };
      return { 2, 3, 4 };
   }

   bool shouldShowControls() const { return showControls; }

   std::string getState() const {
      std::ostringstream oss;
      oss << "; CmdSuper controls\n";
      oss << ";   Reference: Santoro & Mighell, Acta Cryst. A28, 284-287 (1972)\n";
      if (specificOrder > 0) {
         oss << ";   Order: " << specificOrder << "\n";
      } else {
         oss << ";   Orders: 2 3 4 (default)\n";
      }
      return oss.str();
   }

private:
   void setOrder(const int n) {
      if (n == 2 || n == 3 || n == 4) {
         specificOrder = n;
      } else {
         std::cout << "; Warning: ORDER must be 2, 3, or 4 -- ignoring value "
            << n << "\n";
      }
   }

   int specificOrder = 0;   // 0 means "all orders"
};

#endif // CMDSUPER_CONTROLS_H