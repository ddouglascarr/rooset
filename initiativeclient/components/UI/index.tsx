import {h} from 'preact';

type ButtonProps = {
	children: any;
	onClick?: any;
    disabled?: boolean;
	mode?: "default" | "primary";
    type?: "submit"
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
        type={props.type ? props.type : "button"}
	  >
	  	{props.children}
    </button>
	);
};

export const ControlBox = (props: {
  message: string;
  children?: preact.ComponentChildren;
}) => (
  <div class="flex shadow-sm bg-gray-100 border rounded-md p-2 my-2">
    <div class="flex flex-grow flex-col justify-center"><div>{props.message}</div></div>
    <div class="flex space-x-2">
      {props.children}
    </div>
  </div>
);
