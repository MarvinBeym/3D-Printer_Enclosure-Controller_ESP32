import React, {useEffect, useState} from 'react';
import {Field} from "react-final-form";
import {TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	minMaxHelperText: {
		display: "flex",
		justifyContent: "space-between",
	}
}));

const NumberField = ({fieldClassName, name, defaultValue, label, min, max}) => {
	const styles = useStyles();
	const [showMinHelperText, setShowMinHelperText] = useState(false);
	const [showMaxHelperText, setShowMaxHelperText] = useState(false);

	useEffect(() => {
		setShowMaxHelperText(max !== 2147483647);
		setShowMinHelperText(min !== -2147483648);
	}, [max, min]);

	return (
		<Field name={name}
			   defaultValue={defaultValue}
			   render={({input}) => (
				   <TextField
					   type="number"
					   InputProps={{inputProps: {min: min, max: max}}}
					   className={fieldClassName}
					   label={label}
					   helperText={<span className={styles.minMaxHelperText}>{
						   showMinHelperText ? <span>Min: {min}</span> : null
					   } {
						   showMaxHelperText ? <span>Max: {max}</span> : null
					   }</span>}
					   value={input.value}
					   onChange={input.onChange}
					   variant="filled"
				   />
			   )}
		/>
	)
}

export default NumberField