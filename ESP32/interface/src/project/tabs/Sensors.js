import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import {Button} from "@material-ui/core";
import ValueCard from "../components/ValueCard";
import temperatureIcon from "../images/temp_icon_x32.png";
import humidityIcon from "../images/humidity_icon_x32.png";
import Img from "../components/Img";
import {
	clearSensor1Collections,
	clearSensor2Collections,
	selectSensor1,
	selectSensor2
} from "../redux/reducers/sensorsSlice";
import Card from "../components/Card";
import LineChartCard from "../components/LineChartCard";
import TabContent from "../components/TabContent";
import {selectWebinterfaceConfig} from "../redux/reducers/configurationSlice";
import SwitchCard from "../components/SwitchCard";
import {wsSend} from "../redux/reducers/webSocketSlice";
import InputCard from "../components/InputCard";
import {motion} from "framer-motion";

const useStyles = makeStyles(() => ({
	graphSection: {
		display: "flex",
	},
	sensors: {
		display: "grid",
		gridTemplateColumns: "repeat(3, auto)",
		gridTemplateRows: "repeat(2, auto)",
	},
	lineChartContainer: {
		flexDirection: "row",
		justifyContent: "space-between",
	},
	actionsPaper: {
		display: "flex",
		flexDirection: "column",
	},
	sensorPaper: {
		maxWidth: "15rem",
		display: "flex",
		flexDirection: "column",
	},
	lineChartCard: {
		flex: "1",
	},
	sensorCard: {
		display: "grid",
		gridTemplateColumns: "auto auto",
	},
	actionCard: {
		display: "grid",
		gridTemplateRows: "auto auto",
		gap: "1rem",
	},
	tempDangerCard: {
		display: "grid",
		gridTemplateColumns: "1fr 1fr",
	}
}));

const variants = {
	show: {opacity: 1},
	hide: {opacity: 0},
}

const Sensors = () => {
	const dispatch = useDispatch();
	const styles = useStyles();

	const sensor1 = useSelector((state) => selectSensor1(state));
	const sensor2 = useSelector((state) => selectSensor2(state));
	const webinterfaceConfig = useSelector((state) => selectWebinterfaceConfig(state));

	const [sensor1BorderColor, setSensor1BorderColor] = useState("transparent");
	const [sensor2BorderColor, setSensor2BorderColor] = useState("transparent");

	const sensor1DangerThresholdExceeded = sensor1.tempDanger.exceeded;
	const sensor2DangerThresholdExceeded = sensor2.tempDanger.exceeded;

	useEffect(() => {
		if (sensor1DangerThresholdExceeded) {
			setSensor1BorderColor("red");
		} else {
			setSensor1BorderColor("transparent");
		}


	}, [sensor1DangerThresholdExceeded, sensor2DangerThresholdExceeded]);

	useEffect(() => {
		if (sensor2DangerThresholdExceeded) {
			setSensor2BorderColor("red");
		} else {
			setSensor2BorderColor("transparent");
		}
	}, [sensor2DangerThresholdExceeded]);

	const onSensor1TempDangerEnable = (checked) => {
		dispatch(wsSend({component: "sensor1", command: "setTempDangerEnabled", value: checked ? 1 : 0}))
	}

	const onSensor1TempDangerThresholdSave = (value) => {
		dispatch(wsSend({component: "sensor1", command: "setTempDangerThreshold", value: value}))
	}

	const onSensor2TempDangerEnable = (checked) => {
		dispatch(wsSend({component: "sensor2", command: "setTempDangerEnabled", value: checked ? 1 : 0}))
	}

	const onSensor2TempDangerThresholdSave = (value) => {
		dispatch(wsSend({component: "sensor2", command: "setTempDangerThreshold", value: value}))
	}

	return (
		<TabContent className={styles.sensors}>
			<Card header="Sensor 1" className={styles.sensorCard} style={
				{gridArea: '1 / 1 / 2 / 2', border: `solid 4px ${sensor1BorderColor}`}
			}>
				<ValueCard label="Temperature" valueEnding="°C" style={{gridArea: "1 / 1 / 2 / 3"}}
						   endAdornment={<Img src={temperatureIcon}/>}
						   value={sensor1.temperature}/>
				<ValueCard label="Humidity" valueEnding="%" style={{gridArea: "1 / 2 / 2 / 3"}}
						   endAdornment={<Img src={humidityIcon}/>}
						   value={sensor1.humidity}/>

				<Card header="Temperature alarm" className={styles.tempDangerCard} style={{gridArea: "2 / 1 / 3 / 3"}}>
					<SwitchCard header="Enabled" checked={sensor1.tempDanger.enabled}
								onChange={onSensor1TempDangerEnable}/>
					<motion.div animate={sensor1.tempDanger.enabled ? "show" : "hide"} variants={variants}>
						<InputCard header="Threshold" defaultValue={sensor1.tempDanger.threshold}
								   onSave={onSensor1TempDangerThresholdSave}/>
					</motion.div>
				</Card>
			</Card>
			<Card header="Sensor 2" className={styles.sensorCard} style={
				{gridArea: '1 / 2 / 2 / 3', border: `solid 4px ${sensor2BorderColor}`}
			}>
				<ValueCard label="Temperature" valueEnding="°C" style={{gridArea: "1 / 1 / 2 / 3"}}
						   endAdornment={<Img src={temperatureIcon}/>}
						   value={sensor2.temperature}/>
				<ValueCard label="Humidity" valueEnding="%" style={{gridArea: "1 / 2 / 2 / 3"}}
						   endAdornment={<Img src={humidityIcon}/>}
						   value={sensor2.humidity}/>
				<Card header="Temperature alarm" className={styles.tempDangerCard} style={{gridArea: "2 / 1 / 3 / 3"}}>
					<SwitchCard header="Enabled" checked={sensor2.tempDanger.enabled}
								onChange={onSensor2TempDangerEnable}/>
					<motion.div animate={sensor2.tempDanger.enabled ? "show" : "hide"} variants={variants}>
						<InputCard header="Threshold" defaultValue={sensor2.tempDanger.threshold}
								   onSave={onSensor2TempDangerThresholdSave}/>
					</motion.div>
				</Card>

			</Card>
			{webinterfaceConfig.showSensorGraph.sensor1 || webinterfaceConfig.showSensorGraph.sensor2 ? (
				<Card className={styles.actionCard} header="Actions" style={{gridArea: "1 / 3 / 2 / 4"}}>
					{webinterfaceConfig.showSensorGraph.sensor1 ? (
						<Button variant="contained" color="primary"
								onClick={() => dispatch(clearSensor1Collections())}>Clear sensor 1 data</Button>
					) : null}
					{webinterfaceConfig.showSensorGraph.sensor2 ? (
						<Button variant="contained" color="primary"
								onClick={() => dispatch(clearSensor2Collections())}>Clear sensor 2 data</Button>
					) : null}
				</Card>
			) : null}

			{webinterfaceConfig.showSensorGraph.sensor1 || webinterfaceConfig.showSensorGraph.sensor2 ? (
				<Card header="Graphs" className={styles.lineChartContainer} style={{gridArea: "2 / 1 / 3 / 4"}}>
					{webinterfaceConfig.showSensorGraph.sensor1 ? (
						<LineChartCard
							cardClassName={styles.lineChartCard}
							header="Sensor 1"
							yAxisValueSuffix="°C"
							yAxisLabel="Temperature"
							xAxisLabel="Time"
							data={sensor1.temperatureCollection}
							dataKey="temperature"
						/>
					) : null}
					{webinterfaceConfig.showSensorGraph.sensor2 ? (
						<LineChartCard
							cardClassName={styles.lineChartCard}
							header="Sensor 2"
							yAxisValueSuffix="°C"
							yAxisLabel="Temperature"
							xAxisLabel="Time"
							data={sensor2.temperatureCollection}
							dataKey="temperature"
						/>
					) : null}
				</Card>
			) : null}
		</TabContent>

	)
}

export default Sensors;
