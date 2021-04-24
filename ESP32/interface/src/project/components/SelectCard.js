import Card from "./Card";
import {FormControl, InputLabel, MenuItem, Select} from "@material-ui/core";
import React from "react";


const SelectCard = ({header, value, label, onChange, options = []}) => {

	const onSelectChange = (event, value) => {
		onChange(event.target.value);
	}
	return (
		<Card header={header}>
			<FormControl fullWidth variant="filled">
				<InputLabel>{label}</InputLabel>
				<Select defaultValue={options[0].value} value={value} onChange={onSelectChange}>
					{options.map((option, index) => (
						<MenuItem key={index} value={option.value}>{option.label}</MenuItem>
					))}
				</Select>
			</FormControl>
		</Card>
	)
}

export default SelectCard;