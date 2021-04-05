import React from 'react';
import {fetchBackend, useInterval} from "../helper";
import QuickLineChart from "../components/QuickLineChart";
import {makeStyles} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import {
	addSensor1Data,
	addSensor2Data,
	selectSensor1Data,
	selectSensor2Data,
	setSensor1Data,
	setSensor2Data
} from "../redux/reducers/sensorsSlice";
import PaperSection from "../components/PaperSection";
import {Button, Typography} from "@material-ui/core";

const useStyles = makeStyles((theme) => ({
	graphSection: {
		display: "flex",
	},
	currentSensorData: {
		display: "flex",
	},
	actionsPaper: {
		display: "flex",
		flexDirection: "column",
	},
	actionButton: {
		margin: "0.5rem 0",
	}
}));

const Sensors = () => {
	const dispatch = useDispatch();
	const styles = useStyles();

	const sensor1Data = useSelector((state) => selectSensor1Data(state));
	const sensor2Data = useSelector((state) => selectSensor2Data(state));

	const sensor1Temps = sensor1Data.map((data) => {
		return {temperature: data.temperature, time: data.time};
	});
	const sensor2Temps = sensor2Data.map((data) => {
		return {temperature: data.temperature, time: data.time};
	});

	const clearSensor1Data = () => {
		dispatch(setSensor1Data([]));
	}

	const clearSensor2Data = () => {
		dispatch(setSensor2Data([]));
	}

	useInterval(() => {
		fetchBackend("sensors/data").then((response) => {
			let currentTicks = new Date().getTime();
			let sensor1Data = response.sensor1;
			let sensor2Data = response.sensor2;
			sensor1Data.time = currentTicks;
			sensor2Data.time = currentTicks;

			dispatch(addSensor1Data(sensor1Data));
			dispatch(addSensor2Data(sensor2Data));
		})
	}, 2000);

	const currentSensor1Data = sensor1Data[sensor1Temps.length - 1];
	const currentSensor2Data = sensor2Data[sensor2Temps.length - 1];

	return (
		<div>
			<div className={styles.currentSensorData}>
				<PaperSection title="Sensor 1">
					<Typography>Temperature: {currentSensor1Data?.temperature} °C</Typography>
					<Typography>Humidity: {currentSensor1Data?.humidity} %</Typography>
				</PaperSection>
				<PaperSection title="Sensor 2">
					<Typography>Temperature: {currentSensor2Data?.temperature} °C</Typography>
					<Typography>Humidity: {currentSensor2Data?.humidity} %</Typography>
				</PaperSection>
				<PaperSection className={styles.actionsPaper} title="Actions">
					<Button className={styles.actionButton} variant="contained" color="primary"
							onClick={clearSensor1Data}>Clear sensor 1 data</Button>
					<Button className={styles.actionButton} variant="contained" color="primary"
							onClick={clearSensor2Data}>Clear sensor 2 data</Button>
				</PaperSection>
			</div>
			<PaperSection className={styles.graphSection} title="Graphs">
				<QuickLineChart title="Sensor 1" yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time"
								data={sensor1Temps}
								dataKey="temperature"/>
				<QuickLineChart title="Sensor 2" yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time"
								data={sensor2Temps}
								dataKey="temperature"/>
			</PaperSection>
		</div>

	)
}

export default Sensors;
