import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import ValueField from "../components/ValueField";
import temperatureIcon from "../images/temp_icon_x32.png";
import humidityIcon from "../images/humidity_icon_x32.png";
import speedIcon from "../images/speed_icon_x32.png";
import percentIcon from "../images/percent_icon_x32.png";
import Img from "../components/Img";
import {useSelector} from "react-redux";
import {selectFans} from "../redux/reducers/fansSlice";
import {selectSensors} from "../redux/reducers/sensorsSlice";
import {selectLed1, selectLed2} from "../redux/reducers/ledsSlice";
import Card from "../components/Card";
import TabContent from "../components/TabContent";


const useStyles = makeStyles((theme) => ({
	main: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gridTemplateRows: "auto auto auto",
	},
	card: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gap: "1rem",
		flexDirection: "row",
	},
	ledCard: {
		display: "grid",
		gridTemplateColumns: "auto",
		gap: "1rem",
		flexDirection: "row",
	},
	infoPaper: {
		margin: "0",
	},
	infoContainer: {
		display: "flex",
		justifyContent: "space-between",
	},
}));

const Main = () => {
	const styles = useStyles();

	const fans = useSelector((state) => selectFans(state));
	const sensors = useSelector((state) => selectSensors(state));
	const led1 = useSelector((state) => selectLed1(state));
	const led2 = useSelector((state) => selectLed2(state));

	return (
		<TabContent className={styles.main}>
			{fans.map((fan, index) => {
				const header = "Fan " + (index + 1);
				return (
					<Card key={index} className={styles.card} header={header}>
						<ValueField label="Rpm" value={fan.rpm} endAdornment={<Img src={speedIcon}/>}/>
						<ValueField label="Percent" value={fan.percent} endAdornment={<Img src={percentIcon}/>}/>
					</Card>
				)
			})}
			{sensors.map((sensor, index) => {
				const header = "Sensor " + (index + 1);
				return (
					<Card key={index} className={styles.card} title={header}>
						<ValueField label="Temperature" valueEnding="°C" value={sensor.temperature}
									endAdornment={<Img src={temperatureIcon}/>}/>
						<ValueField label="Humidity" valueEnding="%" value={sensor.humidity}
									endAdornment={<Img src={humidityIcon}/>}/>
					</Card>
				)
			})}
			<Card header="Led 1" className={styles.ledCard}>
				<ValueField label="State" value={led1.state ? "ON" : "OFF"}/>
			</Card>
			<Card header="Led 2" className={styles.ledCard}>
				<ValueField label="Effect" value={led2.effects[led2.currentEffect].name.toUpperCase()}/>
			</Card>
		</TabContent>
	)

}

export default Main;
