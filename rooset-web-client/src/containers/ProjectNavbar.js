// @flow

import React from "react";
import { Navbar, Nav, NavItem } from "react-bootstrap";

export default class ProjectNavbar extends React.Component {
  render() {
    return (
      <Navbar inverse>
        <Navbar.Header>
          <Navbar.Brand>
            <a href="#">Rooset</a>
          </Navbar.Brand>
        </Navbar.Header>

        <Nav pullRight>
          <NavItem eventKey={1} href="/login">Login</NavItem>
        </Nav>
      </Navbar>
    );
  }
}
