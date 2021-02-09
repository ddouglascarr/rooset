import {h} from 'preact';

type ButtonProps = {
	children: any;
	onClick?: any;
  disabled?: boolean;
	mode?: "default" | "primary";
}
export const Button = (props: ButtonProps) => {
  let className = "rst-btn";

	if (props.disabled) {
		className += ' rst-btn--disabled';
	}
	if (props.mode && props.mode === "primary") {
		className += ' rst-btn--primary'
	}
 
	return (
	  <button
	  	onClick={props.onClick}
	  		className={className}
	  		disabled={props.disabled}
	  >
	  	{props.children}
    </button>
	);
};
