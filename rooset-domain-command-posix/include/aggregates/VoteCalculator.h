#pragma once
#include <set>
#include <vector>
#include <map>
#include "IssueAggregate.h"
#include "SchulzeBallot.h"
#include "ratk/uuid.h"

using namespace std;
namespace rooset {

  class VoteCalculator
  {
  public:
    virtual ~VoteCalculator() {};

    // Will add status quo (NilId) to possible initiatives
    virtual set<uuid> calcWinners(
        const vector<SchulzeBallot>& ballots,
        const map<uuid, Initiative>& initiatives) =0;
  };

}
