import React from 'react';
import {Field} from "react-final-form";
import {FormControlLabel, Switch} from "@material-ui/core";


const SwitchField = ({fieldClassName, name, defaultValue, label}) => {
	return (
		<Field name={name}
			   type="checkbox"
			   defaultValue={defaultValue}
			   render={({input}) => (
				   <FormControlLabel
					   className={fieldClassName}
					   control={<Switch checked={input.checked} onChange={input.onChange}/>}
					   label={label}
				   />
			   )}
		/>
	)
}

export default SwitchField