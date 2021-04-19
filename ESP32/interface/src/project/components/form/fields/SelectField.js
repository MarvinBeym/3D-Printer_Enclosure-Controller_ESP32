import React from 'react';
import {Field} from "react-final-form";
import {FormControl, InputLabel, MenuItem, Select} from "@material-ui/core";


const SelectField = ({fieldClassName, name, defaultValue, label, options}) => {
	return (
		<Field name={name}
			   defaultValue={defaultValue}
			   render={({input}) => (
				   <FormControl className={fieldClassName} variant="filled">
					   <InputLabel>{label}</InputLabel>
					   <Select value={input.value} onChange={input.onChange}>
						   {options.map((option) => {
							   let optionName = Object.keys(option)[0];
							   let optionValue = option[optionName];
							   return (
								   <MenuItem key={optionValue} value={optionValue}>
									   {optionName.toUpperCase()}
								   </MenuItem>
							   )
						   })}
					   </Select>
				   </FormControl>
			   )}
		/>
	)
}

export default SelectField