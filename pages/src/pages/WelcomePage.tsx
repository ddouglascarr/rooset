import {h, Component} from 'preact';

interface Props {
  Name: string;
}

interface State {}

export default class PrototypePage extends Component<Props, State> {
  render() {
    const {Name} = this.props;
    return <div>Hello {Name}</div>;
  }
}
