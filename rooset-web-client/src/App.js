import React, { Component } from "react";
import ProjectNavbar from "./containers/ProjectNavbar";
import HomePage from './containers/HomePage';
import { BrowserRouter as Router, Route } from "react-router-dom";

class App extends Component {
  render() {
    return (
      <Router>
        <div className="App">
          <ProjectNavbar />
          <Route exact path="/" component={HomePage} />
        </div>
      </Router>
    );
  }
}

export default App;
