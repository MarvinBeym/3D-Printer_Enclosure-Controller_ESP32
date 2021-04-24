import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import SliderCard from "../components/SliderCard";
import {wsSend} from "../redux/reducers/webSocketSlice";
import {useDispatch, useSelector} from "react-redux";
import {selectConfiguration} from "../redux/reducers/configurationSlice";
import IconButtonCard from "../components/IconButtonCard";
import Card from "../components/Card";
import SwitchCard from "../components/SwitchCard";

const useStyles = makeStyles((theme) => ({
	configuration: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gap: "1rem",
	},
	webinterfaceCard: {
		display: "grid",
		gridTemplateColumns: "repeat(2, auto)",
		gap: "1rem",
	},
	esp32Card: {
		display: "grid",
		gridTemplateColumns: "auto max-content",
	}
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
			<Card className={styles.webinterfaceCard} header="Webinterface">
				<SwitchCard header="Fan spin animation"/>
			</Card>
			<Card className={styles.esp32Card} header="ESP32">
				<SliderCard header="Display brightness" value={displayBrightnessSliderValue} onChange={onBrightnessChange}
							onCommit={onBrightnessCommit}/>
				<IconButtonCard header="Save to flash"/>
			</Card>
		</div>
	)
}

export default Configuration;
