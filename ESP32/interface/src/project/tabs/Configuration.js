import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import SliderCard from "../components/SliderCard";
import {wsSend} from "../redux/reducers/webSocketSlice";
import {useDispatch, useSelector} from "react-redux";
import {selectConfiguration} from "../redux/reducers/configurationSlice";

const useStyles = makeStyles((theme) => ({
	configuration: {}
}));

const Configuration = () => {
	const styles = useStyles();
	const dispatch = useDispatch();
	const configuration = useSelector((state) => selectConfiguration(state));

	const [displayBrightnessSliderValue, setDisplayBrightnessSliderValue] = useState(configuration.displayBrightness);

	useEffect(() => {
		setDisplayBrightnessSliderValue(configuration.displayBrightness);
	}, [configuration]);

	const onBrightnessChange = (value) => {
		setDisplayBrightnessSliderValue(value);
	}

	const onBrightnessCommit = (value) => {
		dispatch(wsSend({component: "configuration", command: "setDisplayBrightness", value: value}));
	}

	return (
		<div className={styles.configuration}>
			<SliderCard header="Display brightness" value={displayBrightnessSliderValue} onChange={onBrightnessChange}
						onCommit={onBrightnessCommit}/>
		</div>
	)
}

export default Configuration;
