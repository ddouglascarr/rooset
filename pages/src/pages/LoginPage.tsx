import {h, Component} from 'preact';

interface Props {
  Errors: Array<string>;
  Email: string;
}

interface State {}

const FormField = ({labelText, name, fieldType, value, ...props}: any) => (
  <div className="mt3">
    <label className="db fw6 lh-copy f6" for={name}>
      {labelText}
    </label>
    <input
      className="pa2 input-reset ba bg-transparent hover-bg-black hover-white w-100"
      name={name}
      type={fieldType}
      value={value}
    />
  </div>
);

const Button = ({value, fieldType}: any) => (
  <div className="mv3">
    <input
      className="b ph3 pv2 input-reset ba b--black bg-transparent grow pointer f6 dib"
      type={fieldType}
      value={value}
    />
  </div>
);

const ErrorMsg = ({children}: {children: string}) => (
  <div className="flex items-center justify-center pa3 bg-light-yellow black">
    <span class="lh-title ml3">{children}</span>
  </div>
);

export default class LoginPage extends Component<Props, State> {
  render() {
    const {Email, Errors} = this.props;

    return (
      <main className="pa4 black-80 sans-serif">
        <form className="measure-narrow center" action="/login" method="post">
          <fieldset className="ba b--transparent ph0 mh0">
            <legend className="f4 fw6 ph0 mh0">Sign In</legend>

            {Errors.map(e => (
              <ErrorMsg>{e}</ErrorMsg>
            ))}

            <FormField
              labelText="Email"
              name="Email"
              fieldType="text"
              value={Email}
            />

            <FormField
              labelText="Password"
              name="Password"
              fieldType="password"
              value=""
            />

            <Button value="Login" fieldType="submit" />
          </fieldset>
        </form>
      </main>
    );
  }
}
