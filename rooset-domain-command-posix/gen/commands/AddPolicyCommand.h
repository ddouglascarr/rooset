
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "ratk/ProjectCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "ratk/IdUtils.h"
#include "ratk/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class AddPolicyCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "ADD_POLICY_COMMAND";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid policyId;
        const string name;
        const string description;
        const rooset::VotingAlgorithm votingAlgorithm;
        const unsigned int maxAdmissionTime;
        const unsigned int minAdmissionTime;
        const unsigned int discussionTime;
        const unsigned int verificationTime;
        const unsigned int votingTime;
        const unsigned int issueQuorumNum;
        const unsigned int issueQuorumDen;
        const unsigned int initiativeQuorumNum;
        const unsigned int initiativeQuorumDen;

    
        AddPolicyCommand(
            uuid id,
            uuid requesterId,
            uuid policyId,
            string name,
            string description,
            rooset::VotingAlgorithm votingAlgorithm,
            unsigned int maxAdmissionTime,
            unsigned int minAdmissionTime,
            unsigned int discussionTime,
            unsigned int verificationTime,
            unsigned int votingTime,
            unsigned int issueQuorumNum,
            unsigned int issueQuorumDen,
            unsigned int initiativeQuorumNum,
            unsigned int initiativeQuorumDen) :
            id(id),
            requesterId(requesterId),
            policyId(policyId),
            name(name),
            description(description),
            votingAlgorithm(votingAlgorithm),
            maxAdmissionTime(maxAdmissionTime),
            minAdmissionTime(minAdmissionTime),
            discussionTime(discussionTime),
            verificationTime(verificationTime),
            votingTime(votingTime),
            issueQuorumNum(issueQuorumNum),
            issueQuorumDen(issueQuorumDen),
            initiativeQuorumNum(initiativeQuorumNum),
            initiativeQuorumDen(initiativeQuorumDen)
        {}
  

    
        AddPolicyCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            policyId(JsonUtils::parseUuid(d["payload"]["policyId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            description(JsonUtils::parseString(d["payload"]["description"])),
            votingAlgorithm(EnumUtils::parseVotingAlgorithm(
                JsonUtils::parseString(d["payload"]["votingAlgorithm"]))),
            maxAdmissionTime(d["payload"]["maxAdmissionTime"].GetUint()),
            minAdmissionTime(d["payload"]["minAdmissionTime"].GetUint()),
            discussionTime(d["payload"]["discussionTime"].GetUint()),
            verificationTime(d["payload"]["verificationTime"].GetUint()),
            votingTime(d["payload"]["votingTime"].GetUint()),
            issueQuorumNum(d["payload"]["issueQuorumNum"].GetUint()),
            issueQuorumDen(d["payload"]["issueQuorumDen"].GetUint()),
            initiativeQuorumNum(d["payload"]["initiativeQuorumNum"].GetUint()),
            initiativeQuorumDen(d["payload"]["initiativeQuorumDen"].GetUint())
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

      
          payload.AddMember(
              "id",
              JsonUtils::serializeUuid(id, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "requesterId",
              JsonUtils::serializeUuid(requesterId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "policyId",
              JsonUtils::serializeUuid(policyId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "description",
              JsonUtils::serializeString(description, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "votingAlgorithm",
              JsonUtils::serializeString(
                  EnumUtils::serializeVotingAlgorithm(votingAlgorithm),
                  d->GetAllocator()),
              d->GetAllocator());

          Value maxAdmissionTime_value;
          maxAdmissionTime_value.SetUint(maxAdmissionTime);
          payload.AddMember("maxAdmissionTime", maxAdmissionTime_value, d->GetAllocator());    

          Value minAdmissionTime_value;
          minAdmissionTime_value.SetUint(minAdmissionTime);
          payload.AddMember("minAdmissionTime", minAdmissionTime_value, d->GetAllocator());    

          Value discussionTime_value;
          discussionTime_value.SetUint(discussionTime);
          payload.AddMember("discussionTime", discussionTime_value, d->GetAllocator());    

          Value verificationTime_value;
          verificationTime_value.SetUint(verificationTime);
          payload.AddMember("verificationTime", verificationTime_value, d->GetAllocator());    

          Value votingTime_value;
          votingTime_value.SetUint(votingTime);
          payload.AddMember("votingTime", votingTime_value, d->GetAllocator());    

          Value issueQuorumNum_value;
          issueQuorumNum_value.SetUint(issueQuorumNum);
          payload.AddMember("issueQuorumNum", issueQuorumNum_value, d->GetAllocator());     

          Value issueQuorumDen_value;
          issueQuorumDen_value.SetUint(issueQuorumDen);
          payload.AddMember("issueQuorumDen", issueQuorumDen_value, d->GetAllocator());     

          Value initiativeQuorumNum_value;
          initiativeQuorumNum_value.SetUint(initiativeQuorumNum);
          payload.AddMember("initiativeQuorumNum", initiativeQuorumNum_value, d->GetAllocator());     

          Value initiativeQuorumDen_value;
          initiativeQuorumDen_value.SetUint(initiativeQuorumDen);
          payload.AddMember("initiativeQuorumDen", initiativeQuorumDen_value, d->GetAllocator());     

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};