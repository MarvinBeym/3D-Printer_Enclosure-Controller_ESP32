import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
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
import Card from "../components/Card";
import LineChartCard from "../components/LineChartCard";
import TabContent from "../components/TabContent";
import {selectWebinterfaceConfig} from "../redux/reducers/configurationSlice";

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
		gap: "1rem",
	},
	actionCard: {
		display: "grid",
		gridTemplateRows: "auto auto",
		gap: "1rem",
	}

}));

const Sensors = () => {
	const dispatch = useDispatch();
	const styles = useStyles();

	const sensor1 = useSelector((state) => selectSensor1(state));
	const sensor2 = useSelector((state) => selectSensor2(state));

	const webinterfaceConfig = useSelector((state) => selectWebinterfaceConfig(state));

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
		<TabContent className={styles.sensors}>
			{data.map((sensor, index) => {
				return (
					<Card className={styles.sensorCard} key={sensor.title} style={{gridArea: `1 / ${index + 1} / 2 / ${index + 2}`}}
						  header={sensor.title}>
						{sensor.fields.map((field) => {
							return (
								<ValueField
									key={field.label}
									label={field.label}
									valueEnding={field.ending}
									value={field.value}
									endAdornment={<Img src={field.adornment}/>}
								/>
							)
						})}
					</Card>
				)
			})}
			<Card className={styles.actionCard} header="Actions" style={{gridArea: "1 / 3 / 2 / 4"}}>
				<Button variant="contained" color="primary"
						onClick={() => dispatch(clearSensor1Collections())}>Clear sensor 1 data</Button>
				<Button variant="contained" color="primary"
						onClick={() => dispatch(clearSensor2Collections())}>Clear sensor 2 data</Button>
			</Card>
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
