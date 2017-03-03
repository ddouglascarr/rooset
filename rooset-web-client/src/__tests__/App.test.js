jest.mock("../services/sessionService");

import React from "react";
import ReactDOM from "react-dom";
import App from "../App";
const sessionService: any = require("../services/sessionService").getSession;

it("renders without crashing", () => {
  // sessionService will make a network request on start of app
  sessionService.mockReturnValueOnce(Promise.resolve(null));
  const div = document.createElement("div");
  ReactDOM.render(<App />, div);
});
