import React from "react";
import {TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	valueField: {
		margin: "0 0.5rem",
		"& .MuiOutlinedInput-input": {
			textAlign: "center",
			fontSize: "1.5rem",
			padding: "0.5rem",
			fontWeight: "bolder",
			color: theme.palette.primary.main,
		},
		"& .MuiInputLabel-root": {
			color: theme.palette.primary.main,
			fontSize: "1rem",
		},
		"& .MuiOutlinedInput-root": {
			borderRadius: "2rem",
		},
		borderColor: theme.palette.primary.main,
	},
}));

const ValueField = ({label, value, endAdornment}) => {
	const styles = useStyles();
	return (
		<TextField className={styles.valueField} color="primary" InputProps={{
			readOnly: true
		}} variant="outlined" label={label} value={value}/>
	)
}

export default ValueField;