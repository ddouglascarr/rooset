import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import './style/css/bootstrap-flatly.css'
import { Button } from 'react-bootstrap';

class App extends Component {
  render() {
    return (
      <div className="App">
        <div className="App-header">
          <h2>Welcome to React</h2>
        </div>
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
