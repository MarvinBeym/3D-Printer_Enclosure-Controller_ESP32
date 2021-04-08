import React from 'react';
import QuickLineChart from "../components/QuickLineChart";
import {makeStyles} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import PaperSection from "../components/PaperSection";
import {Button} from "@material-ui/core";
import ValueField from "../components/ValueField";
import temperatureIcon from "../images/temp_icon_x32.png";
import humidityIcon from "../images/humidity_icon_x32.png";
import Img from "../components/Img";
import {
	clearSensor1Collections,
	clearSensor2Collections,
	selectSensor1,
	selectSensor2
} from "../redux/reducers/sensorsSlice";

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

	const sensor1 = useSelector((state) => selectSensor1(state));
	const sensor2 = useSelector((state) => selectSensor2(state));

	const data = [
		{
			title: "Sensor 1", fields: [
				{
					label: "Temperature",
					ending: "°C",
					adornment: temperatureIcon,
					value: sensor1.temperature
				},
				{label: "Humidity", ending: "%", adornment: humidityIcon, value: sensor1.humidity},
			]
		},
		{
			title: "Sensor 2", fields: [
				{
					label: "Temperature",
					ending: "°C",
					adornment: temperatureIcon,
					value: sensor2.temperature
				},
				{label: "Humidity", ending: "%", adornment: humidityIcon, value: sensor2.humidity},
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
							onClick={() => dispatch(clearSensor1Collections())}>Clear sensor 1 data</Button>
					<Button className={styles.actionButton} variant="contained" color="primary"
							onClick={() => dispatch(clearSensor2Collections())}>Clear sensor 2 data</Button>
				</PaperSection>
			</div>
			<PaperSection className={styles.graphSection} title="Graphs">
				<QuickLineChart title="Sensor 1" yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time"
								data={sensor1.temperatureCollection}
								dataKey="temperature"/>
				<QuickLineChart title="Sensor 2" yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time"
								data={sensor2.temperatureCollection}
								dataKey="temperature"/>
			</PaperSection>
		</div>

	)
}

export default Sensors;
