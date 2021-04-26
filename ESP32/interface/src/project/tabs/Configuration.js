import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import SliderCard from "../components/SliderCard";
import {wsSend} from "../redux/reducers/webSocketSlice";
import {useDispatch, useSelector} from "react-redux";
import {
	selectConfiguration,
	selectWebinterfaceConfig,
	setDefaultPage,
	setFanSpinAnimation,
	setShowSensor1Graph,
	setShowSensor2Graph
} from "../redux/reducers/configurationSlice";
import IconButtonCard from "../components/IconButtonCard";
import Card from "../components/Card";
import SwitchCard from "../components/SwitchCard";
import SelectCard from "../components/SelectCard";

const useStyles = makeStyles((theme) => ({
	configuration: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gap: "1rem",
	},
	webinterfaceCard: {
		display: "grid",
		gridTemplateColumns: "repeat(2, auto)",
	},
	esp32Card: {
		display: "grid",
		gridTemplateColumns: "auto max-content",
	}
}));

const defaultPageOptions = [
	{value: "/enclosure-controller/information", label: "Information"},
	{value: "/enclosure-controller/main", label: "Main"},
	{value: "/enclosure-controller/fans", label: "Fans"},
	{value: "/enclosure-controller/sensors", label: "Sensors"},
	{value: "/enclosure-controller/leds", label: "Leds"},
	{value: "/enclosure-controller/configuration", label: "Configuration"},
]

const Configuration = () => {
	const styles = useStyles();
	const dispatch = useDispatch();
	const configuration = useSelector((state) => selectConfiguration(state));
	const webinterfaceConfig = useSelector((state) => selectWebinterfaceConfig(state));
	const [displayBrightnessSliderValue, setDisplayBrightnessSliderValue] = useState(configuration.displayBrightness);

	useEffect(() => {
		setDisplayBrightnessSliderValue(configuration.displayBrightness);
	}, [configuration]);

	const onBrightnessChange = (value) => {
		setDisplayBrightnessSliderValue(value);
	}

	const onDisplaySleepChange = (checked) => {
		dispatch(wsSend({component: "configuration", command: "setDisplaySleep", value: checked ? 1 : 0}));
	}

	const onBrightnessCommit = (value) => {
		dispatch(wsSend({component: "configuration", command: "setDisplayBrightness", value: value}));
	}

	const onDefaultPageChange = (value) => {
		dispatch(setDefaultPage(value));
	}

	const onFanSpinAnimationChange = (checked) => {
		dispatch(setFanSpinAnimation(checked));
	}

	const onSensor1GraphChange = (checked) => {
		dispatch(setShowSensor1Graph(checked));
	}

	const onSensor2GraphChange = (checked) => {
		dispatch(setShowSensor2Graph(checked));
	}

	return (
		<div className={styles.configuration}>
			<Card className={styles.webinterfaceCard} header="Webinterface">
				<SwitchCard
					header="Fan spin animation"
					checked={webinterfaceConfig.fanSpinAnimation}
					onChange={onFanSpinAnimationChange}
				/>
				<SelectCard
					header="Default page"
					options={defaultPageOptions}
					value={webinterfaceConfig.defaultPage}
					onChange={onDefaultPageChange}
				/>
				<SwitchCard
					header="Show sensor 1 Graph"
					checked={webinterfaceConfig.showSensorGraph.sensor1}
					onChange={onSensor1GraphChange}
				/>
				<SwitchCard
					header="Show sensor 2 Graph"
					checked={webinterfaceConfig.showSensorGraph.sensor2}
					onChange={onSensor2GraphChange}
				/>
			</Card>
			<Card className={styles.esp32Card} header="ESP32">
				<SliderCard header="Display brightness" value={displayBrightnessSliderValue}
							onChange={onBrightnessChange}
							onCommit={onBrightnessCommit}/>
				<IconButtonCard header="Save to flash"/>
				<SwitchCard
					header="Display sleep"
					checked={configuration.displaySleep}
					onChange={onDisplaySleepChange}
				/>
			</Card>
		</div>
	)
}

export default Configuration;
