// @flow
import type { State } from "./reducers/rootReducer";
import type { Action } from "./actions/Action";
import type { Store } from "redux";
import React from "react";
import { Provider } from "react-redux";
import ProjectNavbar from "./containers/ProjectNavbar";
import HomePage from "./containers/HomePage";
import LoginPage from "./containers/LoginPage";
import UnitPage from "./containers/UnitPage";
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";
import configureStore from "./store";
import { buildDisplayUnitPageAction } from "./actions/PageAction";

class App extends React.Component {
  state: {
    store: Store<State, Action>,
  };

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
              <Route exact path="/:unitId" component={UnitPage} />
              <Route path="/" render={() => <div>404</div>} />
            </Switch>

            {/* for dispatching redux route events */}
            <Switch>
              <Route exact path="/login" render={() => null} />
              <Route
                exact
                path="/:unitId"
                render={({ match }) => {
                  console.log("unitId path matched");
                  dispatch(
                    buildDisplayUnitPageAction({
                      urlParameterName: match.params.unitId,
                    }),
                  );
                  return null;
                }}
              />
            </Switch>
          </div>
        </Router>
      </Provider>
    );
  }
}

export default App;
