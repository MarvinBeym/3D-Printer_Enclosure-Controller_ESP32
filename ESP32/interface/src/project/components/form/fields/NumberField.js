import React from 'react';
import {Field} from "react-final-form";
import {FormControl, InputLabel, MenuItem, Select, TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	minMaxHelperText: {
		display: "flex",
		justifyContent: "space-between",
	}
}));

const NumberField = ({fieldClassName, name, defaultValue, label, min, max}) => {
	const styles = useStyles();
	return (
		<Field name={name}
			   defaultValue={defaultValue}
			   render={({input}) => (
			   	<TextField
					type="number"
					InputProps={{ inputProps: { min: min, max: max } }}
					className={fieldClassName}
					label={label}
					helperText={<span className={styles.minMaxHelperText}><span>Min: {min}</span><span>Max: {max}</span></span>}
					value={input.value}
					onChange={input.onChange}
					variant="filled"
				/>
			   )}
		/>
	)
}

export default NumberField