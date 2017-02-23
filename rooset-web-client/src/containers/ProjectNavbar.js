// @flow

import React from "react";
import { Navbar, Nav, NavItem } from "react-bootstrap";
import { Link } from "react-router-dom";

export default class ProjectNavbar extends React.Component {
  render() {
    return (
      <Navbar inverse>
        <Navbar.Header>
          <Navbar.Brand>
            <Link to="/">Rooset</Link>
          </Navbar.Brand>
        </Navbar.Header>

        <Nav pullRight>
          <Navbar.Text key={1}>
            <Link to="/login">Login</Link>
          </Navbar.Text>
        </Nav>
      </Navbar>
    );
  }
}
