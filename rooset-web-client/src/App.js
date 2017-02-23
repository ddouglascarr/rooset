import React, { Component } from "react";
import "./style/css/bootstrap.css";
import { Button } from "react-bootstrap";
import ProjectNavbar from "./containers/ProjectNavbar";

class App extends Component {
  render() {
    return (
      <div className="App">
        <ProjectNavbar />
        <div>
          <p className="App-intro">
            <Button
              bsStyle="primary"
              onClick={() => console.log("foo clicked")}
            >
              Foo
            </Button>
          </p>
        </div>
      </div>
    );
  }
}

export default App;
