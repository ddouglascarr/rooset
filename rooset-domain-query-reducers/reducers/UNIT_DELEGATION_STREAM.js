fromCategory("aggregate").foreachStream().when({
  UNIT_DELEGATION_SET_EVENT: linkToUnitDelegations,
  UNIT_DELEGATION_UNSET_EVENT: linkToUnitDelegations,
  AREA_DELEGATION_SET_EVENT: linkToUnitDelegations,
  AREA_DELEGATION_UNSET_EVENT: linkToUnitDelegations,
  DELEGATION_BLOCKED_FOR_AREA_EVENT: linkToUnitDelegations,
  DELEGAITON_UNBLOCKED_FOR_AREA_EVENT: linkToUnitDelegations
});

function linkToUnitDelegations(s, e) {
  const p = e.body.payload;
  linkTo("unitDelegations-" + p.id, e);
}
