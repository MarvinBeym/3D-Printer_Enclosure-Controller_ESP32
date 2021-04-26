import Card from "./Card";
import {Button, TextField} from "@material-ui/core";
import React, {useEffect, useState} from "react";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	minMaxHelperText: {
		display: "flex",
		justifyContent: "space-between",
	},
	inputCard: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gap: "1rem",
	},
	textField: {
		"& .MuiFilledInput-input": {
			padding: "0.5rem 1rem",
		},
		"& .MuiFilledInput-root": {
			height: "100%",
			display: "inline-table",
		}
	}
}));

const InputCard = ({header, defaultValue, onSave, min, max}) => {
	const styles = useStyles();
	const [showMinHelperText, setShowMinHelperText] = useState(false);
	const [showMaxHelperText, setShowMaxHelperText] = useState(false);
	const [inputValue, setInputValue] = useState(defaultValue);

	const onInputChange = (event) => {
		setInputValue(event.target.value);
	}

	const onButtonClick = () => {
		onSave(inputValue);
	}

	useEffect(() => {
		setInputValue(defaultValue);
	}, [defaultValue]);

	useEffect(() => {
		setShowMaxHelperText(!!max);
		setShowMinHelperText(!!min);
	}, [max, min]);

	return (
		<Card header={header} className={styles.inputCard}>
			<TextField
				className={styles.textField}
				type="number"
				InputProps={{inputProps: {min: min, max: max}}}
				helperText={<span className={styles.minMaxHelperText}>{
					showMinHelperText ? <span>Min: {min}</span> : null
				} {
					showMaxHelperText ? <span>Max: {max}</span> : null
				}</span>}
				value={inputValue}
				onChange={onInputChange}
				variant="filled"
			/>
			<Button color="primary" onClick={onButtonClick} variant="contained">Save</Button>
		</Card>
	)
}

export default InputCard;