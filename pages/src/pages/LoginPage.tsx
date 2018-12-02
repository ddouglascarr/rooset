import {h, Component} from 'preact';
import {LoginPageProps} from '../gen/props';

interface State {}

const ErrorMsg = ({children}: {children: string}) => (
  <div className="flex items-center justify-center pa3 bg-light-yellow black">
    <span class="lh-title ml3">{children}</span>
  </div>
);

export default class LoginPage extends Component<LoginPageProps, State> {
  render() {
    const {Errors} = this.props;

    return (
      <main className="pa4 black-80 sans-serif">
        <fieldset className="ba b--transparent ph0 mh0">
          <legend className="f4 fw6 ph0 mh0">Sign In</legend>

          {Errors ? Errors.map(e => <ErrorMsg>{e}</ErrorMsg>) : null}

          <a href="/auth/github/login">Sign In With Github</a>
        </fieldset>
      </main>
    );
  }
}
