#pragma once
#include <memory>
#include <iostream>
#include "aggregates/member/MemberAggregate.h"
#include "framework/uuid.h"

using namespace std;
using namespace rooset;

auto idTools = createIdToolsInstance();

int main()
{
  
  cout << "application running" << "\n";
  cin.get();
  return 0;
}