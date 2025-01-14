// FollowFeatures.cpp
#include "BlockProcessing.h"
#include "FollowFeatures.h"
#include "FollowerMode.h"
#include "InputHandler.h"

//void FollowerControl::writeToStream(std::ostream& os) const {
//   os << ";Follower Mode: " << FollowerControl::mode << "\n";
//   if (perturbations > 1) {
//      os << ";Perturbations: " << perturbations << "\n"
//         << ";Perturb By: " << perturbBy << "\n";
//   }
//   os << ";Follower Points: " << points << "\n";
//}

//FollowControlVariables::FollowControlVariables() {
//   features.push_back(std::make_unique<BlockProcessing>());
//   features.push_back(std::make_unique<FollowerControl>());
//   features.push_back(std::make_unique<FilePrefixControl>());
//}

//void FollowControlVariables::setupHandlers() {
//   InputHandler::clearHandlers();
//
//   InputHandler::registerHandler("BLOCKSIZE", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* blockCtrl = cv.getFeature<BlockProcessing>()) {
//            blockCtrl->setBlockSize(std::stoul(value));
//         }
//      });
//
//   InputHandler::registerHandler("BLOCKSTART", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* blockCtrl = cv.getFeature<BlockProcessing>()) {
//            blockCtrl->setBlockStart(std::stoul(value));
//         }
//      });
//
//   InputHandler::registerHandler("FOLLOWERMODE", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
//            followCtrl->setMode(toFollowerMode(value));
//         }
//      });
//
//   InputHandler::registerHandler("PERTURBATIONS", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
//            followCtrl->setPerturbations(std::stoi(value));
//         }
//      });
//
//   InputHandler::registerHandler("PERTURBBY", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
//            followCtrl->setPerturbBy(std::stod(value));
//         }
//      });
//
//   InputHandler::registerHandler("NUMFOLLOWERPOINTS", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
//            followCtrl->setFollowerPoints(std::stoi(value));
//         }
//      });
//
//   InputHandler::registerHandler("FILEPREFIX", 0.35,
//      [](BaseControlVariables& cv, const std::string& value) {
//         if (auto* fileCtrl = cv.getFeature<FilePrefixControl>()) {
//            fileCtrl->setPrefix(value);
//         }
//      });
//}
