// @flow
global.fetch = jest.fn();

import { getSession } from "../sessionService";
import type { GetSessionResponse } from "../sessionService";

describe("sessionService", () => {
  describe("getSession", () => {
    it("should return null on 401", async () => {
      const mockResponse = new Response(
        `{"timestamp":1488510060258,"status":401,"error":"Unauthorized","message":"Unauthorized","path":"/session"}`,
        {
          status: 401,
          statusText: "Unauthorized",
          headers: new Headers().set("Content-Type", "application/json"),
        },
      );
      const expectedRequest = new Request("/session", {
        method: "GET",
        headers: new Headers().set("Accept", "application/json"),
      });

      fetch.mockReturnValueOnce(Promise.resolve(mockResponse));
      const getSessionResp = await getSession();
      expect(fetch).toHaveBeenCalledWith(expectedRequest);
      expect(getSessionResp).toEqual(null);
    });

    it("should return session object", async () => {
      const mockResponse = new Response(
        JSON.stringify({
          displayName: "Determined Poitras",
          id: "464b1ebb-32c1-460c-8e9e-444444444444",
          username: "determinedPoitras",
        }),
        {
          status: 200,
          headers: new Headers().set("Content-Type", "application/json"),
        },
      );
      const expectedRequest = new Request("/session", {
        method: "GET",
        headers: new Headers().set("Accept", "application/json"),
      });
      const expectedGetSessionResp: GetSessionResponse = {
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      };

      fetch.mockReturnValueOnce(mockResponse);
      const getSessionResp = await getSession();
      expect(fetch).toHaveBeenCalledWith(expectedRequest);
      expect(getSessionResp).toEqual(expectedGetSessionResp);
    });
  });
});
