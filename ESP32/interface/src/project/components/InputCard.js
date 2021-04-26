import Card from "./Card";
import {TextField} from "@material-ui/core";
import React, {useEffect, useState} from "react";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	minMaxHelperText: {
		display: "flex",
		justifyContent: "space-between",
	}
}));

const InputCard = ({header, value, onChange, min, max}) => {
	const styles = useStyles();
	const [showMinHelperText, setShowMinHelperText] = useState(false);
	const [showMaxHelperText, setShowMaxHelperText] = useState(false);

	const onInputChange = (event) => {
		onChange(event.target.value);
	}

	useEffect(() => {
		setShowMaxHelperText(!!max);
		setShowMinHelperText(!!min);
	}, [max, min]);

	return (
		<Card header={header}>
			<TextField
				type="number"
				InputProps={{inputProps: {min: min, max: max}}}
				helperText={<span className={styles.minMaxHelperText}>{
					showMinHelperText ? <span>Min: {min}</span> : null
				} {
					showMaxHelperText ? <span>Max: {max}</span> : null
				}</span>}
				value={value}
				onChange={onInputChange}
				variant="filled"
			/>
		</Card>
	)
}

export default InputCard;