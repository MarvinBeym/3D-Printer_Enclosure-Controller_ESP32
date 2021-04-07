import React, {useState} from 'react';
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

	const [currentSensor1Data, setCurrentSensor1Data] = useState(null);
	const [currentSensor2Data, setCurrentSensor2Data] = useState(null);

	const [sensor1Temps, setSensor1Temps] = useState([]);
	const [sensor2Temps, setSensor2Temps] = useState([]);

	const sensor1Data = useSelector((state) => selectSensor1Data(state));
	const sensor2Data = useSelector((state) => selectSensor2Data(state));

	const clearSensor1Data = () => {
		dispatch(setSensor1Data([]));
	}

	const clearSensor2Data = () => {
		dispatch(setSensor2Data([]));
	}

	useInterval(() => {
		fetchBackend("sensors/data").then((response) => {
			let currentTicks = new Date().getTime();
			let newSensor1Data = response.sensor1;
			let newSensor2Data = response.sensor2;
			newSensor1Data.time = currentTicks;
			newSensor2Data.time = currentTicks;

			setCurrentSensor1Data(newSensor1Data);
			setCurrentSensor2Data(newSensor2Data);

			if (sensor1Data.length === 0 || newSensor1Data.temperature !== sensor1Data[sensor1Data.length - 1].temperature) {
				setSensor1Temps([...sensor1Temps, {temperature: newSensor1Data.temperature, time: newSensor1Data.time}])
			}

			if (sensor2Data.length === 0 || newSensor2Data.temperature !== sensor2Data[sensor2Data.length - 1].temperature) {
				setSensor2Temps([...sensor2Temps, {temperature: newSensor2Data.temperature, time: newSensor2Data.time}])
			}

			dispatch(addSensor1Data(newSensor1Data));
			dispatch(addSensor2Data(newSensor2Data));
		})
	}, 2000);

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
						<PaperSection key={sensor.title} className={styles.sensorPaper} title={sensor.title}>
							{sensor.fields.map((field) => {
								return (
									<ValueField
										key={field.label}
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
