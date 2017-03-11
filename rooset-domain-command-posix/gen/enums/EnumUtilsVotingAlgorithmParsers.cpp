
#include "enums/EnumUtils.h"
#include "enums/VotingAlgorithm.h"
#include <stdexcept>

namespace rooset {
  VotingAlgorithm EnumUtils::parseVotingAlgorithm(const string& e)
  {
    if (e == "SCHULZE") return VotingAlgorithm::SCHULZE;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeVotingAlgorithm(VotingAlgorithm e)
  {
    if (e == VotingAlgorithm::SCHULZE) return "SCHULZE";
    throw std::invalid_argument("unknown enum");
  }

}  