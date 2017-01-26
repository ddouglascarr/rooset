#include <string>
#include "uuid.h"


namespace rooset {

  class CommandService {
  public:
    virtual ~CommandService() {};
    virtual uuid evaluate(const string& sJson) =0;
  };

}


