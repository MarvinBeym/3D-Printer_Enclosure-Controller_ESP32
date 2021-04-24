import {Slider} from "@material-ui/core";
import Card from "./Card";
import React, {useEffect, useState} from "react";


const SliderCard = ({header, onChange, onCommit, min = 0, max = 100, step = 1, marks, value}) => {
	const [sliderMarks, setSliderMarks] = useState([]);

	const onSliderChange = (event, value) => {
		onChange(value);
	}

	const onSliderChangeCommit = (event, value) => {
		onCommit(value);
	}

	useEffect(() => {
		if (!marks) {
			setSliderMarks([
				{value: 0, label: "0%"},
				{value: 25, label: "25%"},
				{value: 50, label: "50%"},
				{value: 75, label: "75%"},
				{value: 100, label: "100%"},
			]);
		} else {
			setSliderMarks(marks);
		}
	}, [marks]);

	return (
		<Card header={header}>
			<Slider min={min} max={max} step={step}
					onChange={onSliderChange}
					onChangeCommitted={onSliderChangeCommit}
					marks={sliderMarks} valueLabelDisplay="auto"
					value={value}/>
		</Card>
	)
}

export default SliderCard;