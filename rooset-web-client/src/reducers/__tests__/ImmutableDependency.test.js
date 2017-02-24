import {
  buildDisplayUnitPageAction,
  buildDisplayUserPageAction,
} from "../../actions/PageAction";

describe("action declarations", () => {
  it("flow should pass these", () => {
    const action = buildDisplayUnitPageAction("foobar");
    expect(action.payload.id).toEqual("foobar");
  });

  it("flow should fail this one", () => {
    const action = buildDisplayUserPageAction("foo");
    expect(action.payload.id).toEqual("foo");
  });
});
