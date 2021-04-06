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
import {Button} from "@material-ui/core";
import ValueField from "../components/ValueField";
import temperatureIcon from "../images/temp_icon_x32.png";
import humidityIcon from "../images/humidity_icon_x32.png";
import Img from "../components/Img";

const useStyles = makeStyles(() => ({
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
	},
	sensorPaper: {
		maxWidth: "15rem",
		display: "flex",
		flexDirection: "column",
	},
	valueField: {
		margin: "0.5rem 0",
	},
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

	const data = [
		{
			title: "Sensor 1", fields: [
				{
					label: "Temperature",
					ending: "°C",
					adornment: temperatureIcon,
					value: currentSensor1Data?.temperature
				},
				{label: "Humidity", ending: "%", adornment: humidityIcon, value: currentSensor1Data?.humidity},
			]
		},
		{
			title: "Sensor 2", fields: [
				{
					label: "Temperature",
					ending: "°C",
					adornment: temperatureIcon,
					value: currentSensor2Data?.temperature
				},
				{label: "Humidity", ending: "%", adornment: humidityIcon, value: currentSensor2Data?.humidity},
			]
		}
	];

	return (
		<div>
			<div className={styles.currentSensorData}>
				{data.map((sensor) => {
					return (
						<PaperSection className={styles.sensorPaper} title={sensor.title}>
							{sensor.fields.map((field) => {
								return (
									<ValueField
										className={styles.valueField}
										label={field.label}
										valueEnding={field.ending}
										value={field.value}
										endAdornment={<Img src={field.adornment}/>}
									/>
								)
							})}
						</PaperSection>
					)
				})}
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
