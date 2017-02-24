import React, { Component } from "react";
import ProjectNavbar from "./containers/ProjectNavbar";
import HomePage from "./containers/HomePage";
import LoginPage from "./containers/LoginPage";
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";

class App extends Component {
  render() {
    return (
      <Router>
        <div className="App">
          <ProjectNavbar />
          <Switch>
            <Route exact path="/" component={HomePage} />
            <Route
              exact
              path="/login"
              component={LoginPage}
              onEnter={(nextState, replace, cb) => {
                console.log(nextState);
                console.log("foobar");
                cb();
              }}
              componentDidUpdate={() => console.log("login did mount")}
            />
            <Route path="/" render={() => <div>404</div>} />
          </Switch>

          {/* for dispatching redux route events */}
          <Route
            path="/"
            render={(props) => {
              console.log("path changed");
              console.log(props.location.pathname);
              return null;
            }}
          />
        </div>
      </Router>
    );
  }
}

export default App;
