#pragma once
#include <vector>
#include "ratk/uuid.h"
#include "rapidjson/document.h"

using namespace std;
namespace rooset {

  class SchulzeBallot {
  private:
    IdToolsImpl idTools;
    vector<vector<uuid>> schulzeRanking;
    unsigned long long weight = 0;
  public:
    const vector<vector<uuid>> approve;
    const vector<uuid> abstain;
    const vector<vector<uuid>> disapprove;

    SchulzeBallot(
        const vector<vector<uuid>>& approve,
        const vector<uuid>& abstain,
        const vector<vector<uuid>>& disapprove);

    inline auto getSchulzeRanking() const { return schulzeRanking; }
    inline void setWeight(unsigned long long w) { weight = w; }
    inline auto getWeight() const { return weight; }

    static SchulzeBallot parse(const rapidjson::Value& v);
    static rapidjson::Value serialize(
        const SchulzeBallot& b, rapidjson::Document::AllocatorType& allocator);
  };

}
