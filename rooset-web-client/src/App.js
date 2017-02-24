import React, { Component } from "react";
import { Provider } from "react-redux";
import ProjectNavbar from "./containers/ProjectNavbar";
import HomePage from "./containers/HomePage";
import LoginPage from "./containers/LoginPage";
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";
import configureStore from "./store";

class App extends Component {
  constructor() {
    super();
    this.state = {
      store: configureStore(new Map()),
    };
  }
  render() {
    const { dispatch } = this.state.store;
    return (
      <Provider store={this.state.store}>
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
              render={props => {
                dispatch({
                  type: "ROUTE_CHANGED",
                  payload: { pathname: props.location.pathname },
                });
                return null;
              }}
            />
          </div>
        </Router>
      </Provider>
    );
  }
}

export default App;
