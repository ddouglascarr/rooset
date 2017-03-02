import React from "react";
import {
  Page,
  PageBody,
  Content,
  LeftSideBar,
  Text,
} from "../components/BaseComponents";
import Navbar from "../components/Navbar";

export default class HomePage extends React.Component {
  render() {
    return (
      <Page>
        <Navbar />
        <PageBody>
          <LeftSideBar />
          <Content>
            <Text weight="semibold" size="xxl">Welcome to Rooset</Text>
          </Content>
        </PageBody>
      </Page>
    );
  }
}
