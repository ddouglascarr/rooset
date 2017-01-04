#include "VoteCalculator.h"
#include <vector>


rooset::VoteCalculator::SchulzeBallot::SchulzeBallot(
    const vector<vector<uuid>>& approve,
    const vector<uuid>& inAbstain,
    const vector<vector<uuid>>& disapprove)
{
  schulzeRanking.insert(schulzeRanking.end(), approve.begin(), approve.end());
  auto abstain = inAbstain;
  abstain.push_back(idTools.generateNilId());
  schulzeRanking.push_back({ abstain });
  schulzeRanking.insert(schulzeRanking.end(), disapprove.begin(), disapprove.end());
}
  

