// FollowFeatures.cpp
#include "FollowFeatures.h"
#include "InputHandler.h"

void FollowerControl::writeToStream(std::ostream& os) const {
   os << ";Follower Mode: " << mode << "\n";
   if (perturbations > 1) {
      os << ";Perturbations: " << perturbations << "\n"
         << ";Perturb By: " << perturbBy << "\n";
   }
   os << ";Follower Points: " << followerPoints << "\n";
}

void FollowerControl::setMode(FollowerMode newMode) { mode = newMode; }
void FollowerControl::setPerturbations(int count) { perturbations = count; }
void FollowerControl::setPerturbBy(double amount) { perturbBy = amount; }
void FollowerControl::setFollowerPoints(int points) { followerPoints = points; }
FollowerMode FollowerControl::getMode() const { return mode; }
int FollowerControl::getPerturbations() const { return perturbations; }
double FollowerControl::getPerturbBy() const { return perturbBy; }
int FollowerControl::getFollowerPoints() const { return followerPoints; }

void FilePrefixControl::writeToStream(std::ostream& os) const {
   os << ";File Prefix: " << prefix << "\n";
}
void FilePrefixControl::setPrefix(const std::string& newPrefix) { prefix = newPrefix; }
std::string FilePrefixControl::getPrefix() const { return prefix; }

FollowControlVariables::FollowControlVariables() {
   features.push_back(std::make_unique<BlockControl>());
   features.push_back(std::make_unique<FollowerControl>());
   features.push_back(std::make_unique<FilePrefixControl>());
}

void FollowControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("BLOCKSIZE", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* blockCtrl = cv.getFeature<BlockControl>()) {
            blockCtrl->setBlockSize(std::stoul(value));
         }
      });

   InputHandler::registerHandler("BLOCKSTART", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* blockCtrl = cv.getFeature<BlockControl>()) {
            blockCtrl->setBlockStart(std::stoul(value));
         }
      });

   InputHandler::registerHandler("FOLLOWERMODE", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
            followCtrl->setMode(toFollowerMode(value));
         }
      });

   InputHandler::registerHandler("PERTURBATIONS", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
            followCtrl->setPerturbations(std::stoi(value));
         }
      });

   InputHandler::registerHandler("PERTURBBY", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
            followCtrl->setPerturbBy(std::stod(value));
         }
      });

   InputHandler::registerHandler("NUMFOLLOWERPOINTS", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
            followCtrl->setFollowerPoints(std::stoi(value));
         }
      });

   InputHandler::registerHandler("FILEPREFIX", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* fileCtrl = cv.getFeature<FilePrefixControl>()) {
            fileCtrl->setPrefix(value);
         }
      });
}
