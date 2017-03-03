// @flow

global.fetch = jest.fn();

describe("dependencies", () => {
  describe("fetch", () => {
    it("mock fetch", async () => {
      const mockResponse = new Response(JSON.stringify({ foo: "bar" }), {
        status: 200,
        statusText: "OK mock test",
        headers: new Headers().set("Content-Type", "application/json"),
      });
      const wrongStatusResponse = new Response(JSON.stringify({ foo: "bar" }), {
        status: 401,
        statusText: "OK mock test",
        headers: new Headers().set("Content-Type", "application/json"),
      });
      const wrongBodyResponse = new Response(
        JSON.stringify({ foo: "notbar" }),
        {
          status: 200,
          statusText: "OK mock test",
          headers: new Headers().set("Content-Type", "application/json"),
        },
      );
      fetch.mockReturnValueOnce(Promise.resolve(mockResponse));

      const resp = await fetch("/foobar");
      expect(resp).toEqual(mockResponse);
      expect(resp).not.toEqual(wrongStatusResponse);
      expect(resp).not.toEqual(wrongBodyResponse);
    });

    it("compare fetch requests", async () => {
      const mockResponse = new Response(JSON.stringify({ foo: "bar" }), {
        status: 200,
        statusText: "OK mock test",
        headers: new Headers().set("Content-Type", "application/json"),
      });
      const expectedRequest = new Request("/foo", {
        method: "POST",
        body: JSON.stringify({ foo: "bar" }),
        headers: new Headers().set("Content-Type", "application/json"),
      });
      const wrongBodyRequest = new Request("/foo", {
        method: "POST",
        body: JSON.stringify({ foo: "notbar" }),
        headers: new Headers().set("Content-Type", "application/json"),
      });

      fetch.mockReturnValueOnce(Promise.resolve(mockResponse));
      const resp = await fetch(
        new Request("/foo", {
          method: "POST",
          headers: new Headers().set("Content-Type", "application/json"),
          body: JSON.stringify({ foo: "bar" }),
        }),
      );

      expect(fetch).toHaveBeenCalledWith(expectedRequest);
      expect(fetch).not.toHaveBeenCalledWith(wrongBodyRequest);
    });
  });
});
