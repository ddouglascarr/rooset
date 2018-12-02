import {h, Component} from 'preact';

interface Props {
  GithubClientID: string;
  GithubCallbackURI: string;
}

interface State {}

export default class HomePage extends Component<Props, State> {
  render() {
    const {GithubClientID, GithubCallbackURI} = this.props;

    return (
      <main className="pa4 black-80 sans-serif">
        <div>
          <a href="/auth/github/login">Sign In With Github</a>
        </div>
        <div>
          <h1>Rooset</h1>
        </div>
      </main>
    );
  }
}
