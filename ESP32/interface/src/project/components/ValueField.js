import React from "react";
import {InputAdornment, TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";

const useStyles = makeStyles((theme) => ({
	valueField: {
		margin: "0 0.5rem",
		"& .MuiInputBase-root": {
			padding: "0.5rem",
		},
		"& .MuiOutlinedInput-input": {
			textAlign: "center",
			fontSize: "1.5rem",
			fontWeight: "bolder",
			padding: "0",
			color: theme.palette.primary.main,
		},
		"& .MuiInputLabel-root": {
			color: theme.palette.primary.main,
			fontSize: "1rem",
		},
		"& .MuiOutlinedInput-root": {
			borderRadius: "2rem",
		},
		"& .MuiOutlinedInput-notchedOutline": {
			borderColor: theme.palette.primary.main,
			borderWidth: "2px",
		},
	},
}));

const ValueField = ({label, value, className, endAdornment}) => {
	const styles = useStyles();
	return (
		<TextField className={clsx(styles.valueField, className)} color="primary" InputProps={{
			endAdornment: endAdornment ? <InputAdornment position="end">{endAdornment}</InputAdornment> : null,
			readOnly: true
		}} variant="outlined" label={label} value={value}/>
	)
}

export default ValueField;