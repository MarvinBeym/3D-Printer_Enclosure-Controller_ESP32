import React from "react";
import {InputAdornment, TextField} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";
import Card from "./Card";

const useStyles = makeStyles((theme) => ({
	valueField: {
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
	},
	card: {
		width: "100%",
		padding: "0.5rem",
	},
	cardContent: {
		padding: "0",
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

const ValueCard = ({label, value, className, endAdornment, valueEnding}) => {
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
		<Card cardClassName={styles.card} className={styles.cardContent} elevation={5}>
			<TextField className={clsx(styles.valueField, className)} color="primary" InputProps={{
				endAdornment: endAdor,
				readOnly: true,
			}} variant="outlined" label={label} value={value ? value : typeof value == "number" ? value : " "}/>
		</Card>

	)
}

export default ValueCard;