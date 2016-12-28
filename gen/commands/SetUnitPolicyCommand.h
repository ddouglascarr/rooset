
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "framework/uuid.h"
#include "framework/ProjectCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "framework/IdToolsImpl.h"
#include "framework/JsonUtils.h"
#include "enums/EnumUtils.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class SetUnitPolicyCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "SET_UNIT_POLICY_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const uuid policyId;
        const string name;
        const string description;
        const bool polling;
        const boost::posix_time::time_duration maxAdmissionTime;
        const boost::posix_time::time_duration minAdmissionTime;
        const boost::posix_time::time_duration discussionTime;
        const boost::posix_time::time_duration verificationTime;
        const boost::posix_time::time_duration votingTime;
        const unsigned int issueQuorumNum;
        const unsigned int issueQuorumDen;
        const rooset::DefeatStrength defeatStrength;
        const unsigned int directMajorityNum;
        const unsigned int directMajorityDen;
        const bool directMajorityStrict;
        const unsigned int directMajorityPositive;
        const unsigned int directMajorityNonNegative;
        const bool noReverseBeatPath;
        const bool noMultistageMajority;

    
        SetUnitPolicyCommand(
            uuid id,
            uuid requesterId,
            uuid policyId,
            string name,
            string description,
            bool polling,
            boost::posix_time::time_duration maxAdmissionTime,
            boost::posix_time::time_duration minAdmissionTime,
            boost::posix_time::time_duration discussionTime,
            boost::posix_time::time_duration verificationTime,
            boost::posix_time::time_duration votingTime,
            unsigned int issueQuorumNum,
            unsigned int issueQuorumDen,
            rooset::DefeatStrength defeatStrength,
            unsigned int directMajorityNum,
            unsigned int directMajorityDen,
            bool directMajorityStrict,
            unsigned int directMajorityPositive,
            unsigned int directMajorityNonNegative,
            bool noReverseBeatPath,
            bool noMultistageMajority) :
            id(id) ,
            requesterId(requesterId) ,
            policyId(policyId) ,
            name(name) ,
            description(description) ,
            polling(polling),
            maxAdmissionTime(maxAdmissionTime) ,
            minAdmissionTime(minAdmissionTime) ,
            discussionTime(discussionTime) ,
            verificationTime(verificationTime) ,
            votingTime(votingTime) ,
            issueQuorumNum(issueQuorumNum),
            issueQuorumDen(issueQuorumDen),
            defeatStrength(defeatStrength),
            directMajorityNum(directMajorityNum),
            directMajorityDen(directMajorityDen),
            directMajorityStrict(directMajorityStrict),
            directMajorityPositive(directMajorityPositive),
            directMajorityNonNegative(directMajorityNonNegative),
            noReverseBeatPath(noReverseBeatPath),
            noMultistageMajority(noMultistageMajority)
        {}
  

    
        SetUnitPolicyCommand(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            policyId(idTools->parse(string(
                d["payload"]["policyId"].GetString(),
                d["payload"]["policyId"].GetStringLength()))),
            name(string(
                d["payload"]["name"].GetString(),
                d["payload"]["name"].GetStringLength())),
            description(string(
                d["payload"]["description"].GetString(),
                d["payload"]["description"].GetStringLength())),
            polling(d["payload"]["polling"].GetBool()),
            maxAdmissionTime(boost::posix_time::seconds(d["payload"]["maxAdmissionTime"].GetDouble())),
            minAdmissionTime(boost::posix_time::seconds(d["payload"]["minAdmissionTime"].GetDouble())),
            discussionTime(boost::posix_time::seconds(d["payload"]["discussionTime"].GetDouble())),
            verificationTime(boost::posix_time::seconds(d["payload"]["verificationTime"].GetDouble())),
            votingTime(boost::posix_time::seconds(d["payload"]["votingTime"].GetDouble())),
            issueQuorumNum(d["payload"]["issueQuorumNum"].GetUint()),
            issueQuorumDen(d["payload"]["issueQuorumDen"].GetUint()),
            defeatStrength(EnumUtils::parseDefeatStrength(string(
                d["payload"]["defeatStrength"].GetString(),
                d["payload"]["defeatStrength"].GetStringLength()))),
            directMajorityNum(d["payload"]["directMajorityNum"].GetUint()),
            directMajorityDen(d["payload"]["directMajorityDen"].GetUint()),
            directMajorityStrict(d["payload"]["directMajorityStrict"].GetBool()),
            directMajorityPositive(d["payload"]["directMajorityPositive"].GetUint()),
            directMajorityNonNegative(d["payload"]["directMajorityNonNegative"].GetUint()),
            noReverseBeatPath(d["payload"]["noReverseBeatPath"].GetBool()),
            noMultistageMajority(d["payload"]["noMultistageMajority"].GetBool())
        {}
  

    unique_ptr<Document> serialize() const override
    {
      auto d = make_unique<Document>();
      d->SetObject();

      Value payload;
      payload.SetObject();

      Value messageTypeValue;
      messageTypeValue.SetString(MESSAGE_TYPE.c_str(),
          MESSAGE_TYPE.size(), d->GetAllocator());
      d->AddMember("type", messageTypeValue, d->GetAllocator());

      
          Value id_value;
          string id_str = idTools->serialize(id);
          id_value.SetString(id_str.c_str(), id_str.size(), d->GetAllocator());
          payload.AddMember("id", id_value, d->GetAllocator());    

          Value requesterId_value;
          string requesterId_str = idTools->serialize(requesterId);
          requesterId_value.SetString(requesterId_str.c_str(), requesterId_str.size(), d->GetAllocator());
          payload.AddMember("requesterId", requesterId_value, d->GetAllocator());    

          Value policyId_value;
          string policyId_str = idTools->serialize(policyId);
          policyId_value.SetString(policyId_str.c_str(), policyId_str.size(), d->GetAllocator());
          payload.AddMember("policyId", policyId_value, d->GetAllocator());    

          Value name_value;
          name_value.SetString(name.c_str(), name.size(), d->GetAllocator());
          payload.AddMember("name", name_value, d->GetAllocator());    

          Value description_value;
          description_value.SetString(description.c_str(), description.size(), d->GetAllocator());
          payload.AddMember("description", description_value, d->GetAllocator());    

          Value polling_value;
          polling_value.SetBool(polling);
          payload.AddMember("polling", polling_value, d->GetAllocator());

          Value maxAdmissionTime_value;
          maxAdmissionTime_value.SetDouble(maxAdmissionTime.seconds());
          payload.AddMember("maxAdmissionTime", maxAdmissionTime_value, d->GetAllocator());    

          Value minAdmissionTime_value;
          minAdmissionTime_value.SetDouble(minAdmissionTime.seconds());
          payload.AddMember("minAdmissionTime", minAdmissionTime_value, d->GetAllocator());    

          Value discussionTime_value;
          discussionTime_value.SetDouble(discussionTime.seconds());
          payload.AddMember("discussionTime", discussionTime_value, d->GetAllocator());    

          Value verificationTime_value;
          verificationTime_value.SetDouble(verificationTime.seconds());
          payload.AddMember("verificationTime", verificationTime_value, d->GetAllocator());    

          Value votingTime_value;
          votingTime_value.SetDouble(votingTime.seconds());
          payload.AddMember("votingTime", votingTime_value, d->GetAllocator());    

          Value issueQuorumNum_value;
          issueQuorumNum_value.SetUint(issueQuorumNum);
          payload.AddMember("issueQuorumNum", issueQuorumNum_value, d->GetAllocator());     

          Value issueQuorumDen_value;
          issueQuorumDen_value.SetUint(issueQuorumDen);
          payload.AddMember("issueQuorumDen", issueQuorumDen_value, d->GetAllocator());     

          Value defeatStrength_value;
          auto defeatStrength_str = EnumUtils::serializeDefeatStrength(defeatStrength);
          defeatStrength_value.SetString(defeatStrength_str.c_str(), defeatStrength_str.size(), d->GetAllocator());
          payload.AddMember("defeatStrength", defeatStrength_value, d->GetAllocator());    

          Value directMajorityNum_value;
          directMajorityNum_value.SetUint(directMajorityNum);
          payload.AddMember("directMajorityNum", directMajorityNum_value, d->GetAllocator());     

          Value directMajorityDen_value;
          directMajorityDen_value.SetUint(directMajorityDen);
          payload.AddMember("directMajorityDen", directMajorityDen_value, d->GetAllocator());     

          Value directMajorityStrict_value;
          directMajorityStrict_value.SetBool(directMajorityStrict);
          payload.AddMember("directMajorityStrict", directMajorityStrict_value, d->GetAllocator());

          Value directMajorityPositive_value;
          directMajorityPositive_value.SetUint(directMajorityPositive);
          payload.AddMember("directMajorityPositive", directMajorityPositive_value, d->GetAllocator());     

          Value directMajorityNonNegative_value;
          directMajorityNonNegative_value.SetUint(directMajorityNonNegative);
          payload.AddMember("directMajorityNonNegative", directMajorityNonNegative_value, d->GetAllocator());     

          Value noReverseBeatPath_value;
          noReverseBeatPath_value.SetBool(noReverseBeatPath);
          payload.AddMember("noReverseBeatPath", noReverseBeatPath_value, d->GetAllocator());

          Value noMultistageMajority_value;
          noMultistageMajority_value.SetBool(noMultistageMajority);
          payload.AddMember("noMultistageMajority", noMultistageMajority_value, d->GetAllocator());

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};
  