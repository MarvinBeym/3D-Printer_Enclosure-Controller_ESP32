import React from "react";
import {InputAdornment, TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";

const useStyles = makeStyles((theme) => ({
	valueField: {
		margin: "0 0.5rem",
		width: "100%",
		"& .MuiInputBase-root": {
			padding: "0.5rem",
		},
		"& .MuiOutlinedInput-input": {
			cursor: "default",
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
			cursor: "default",
		},
		"& .MuiOutlinedInput-notchedOutline": {
			borderColor: theme.palette.primary.main,
			borderWidth: "2px",
		},
	},
	valueEnding: {
		color: theme.palette.primary.main,
		fontWeight: "bolder",
		fontSize: "1.5rem",
		marginRight: "0.5rem",
	}
}));

const ValueEnding = ({value}) => {
	const styles = useStyles();
	return (
		<div className={styles.valueEnding}>
			{value}
		</div>
	)
}

const ValueField = ({label, value, className, endAdornment, valueEnding}) => {
	const styles = useStyles();

	let endAdor = null;
	if (endAdornment && !valueEnding) {
		endAdor = <InputAdornment position="end">{endAdornment}</InputAdornment>;
	} else if (!endAdornment && valueEnding) {
		endAdor = <InputAdornment position="end"><ValueEnding value={valueEnding}/></InputAdornment>;
	} else if (endAdornment && valueEnding) {
		endAdor = <InputAdornment position="end"><ValueEnding value={valueEnding}/> {endAdornment}</InputAdornment>;
	}

	return (
		<TextField className={clsx(styles.valueField, className)} color="primary" InputProps={{
			endAdornment: endAdor,
			readOnly: true,
		}} variant="outlined" label={label} value={value ? value : typeof value == "number" ? value : " "}/>
	)
}

export default ValueField;