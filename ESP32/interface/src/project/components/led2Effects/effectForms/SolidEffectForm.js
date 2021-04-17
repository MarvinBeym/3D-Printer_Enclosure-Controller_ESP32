import {FormControl, InputLabel, MenuItem, Select} from "@material-ui/core";
import React, {useState} from "react";


const SolidEffectForm = ({configSetup}) => {
	const [selectedColor, setSelectedColor] = useState(configSetup.colors[0][Object.keys(configSetup.colors[0])])

	const onColorSelectChange = (event) => {
		setSelectedColor(event.target.value);
	}


	return (
		<div>
			<FormControl fullWidth variant="filled">
				<InputLabel id="demo-simple-select-outlined-label">Color</InputLabel>
				<Select value={selectedColor} onChange={onColorSelectChange}>
					{configSetup.colors.map((colorObject) => {
						let colorName = Object.keys(colorObject)[0];
						let colorHtmlCode = colorObject[colorName];
						return <MenuItem key={colorHtmlCode} value={colorHtmlCode}>{colorName.toUpperCase()}</MenuItem>
					})}
				</Select>
			</FormControl>
		</div>
	)
}

export default SolidEffectForm;