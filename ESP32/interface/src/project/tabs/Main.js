import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import PaperSection from "../components/PaperSection";
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


const useStyles = makeStyles((theme) => ({
	main: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gridTemplateRows: "auto auto auto",
		gridGap: "1rem",
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
		<PaperSection className={styles.main}>
			{fans.map((fan, index) => (
				<PaperSection
					paperClassName={styles.infoPaper}
					className={styles.infoContainer}
					title={"Fan " + (index + 1)}
				>
					<ValueField label="Rpm" value={fan.rpm} endAdornment={<Img src={speedIcon}/>}/>
					<ValueField label="Percent" value={fan.percent} endAdornment={<Img src={percentIcon}/>}/>
				</PaperSection>
			))}
			{sensors.map((sensor, index) => (
				<PaperSection
					paperClassName={styles.infoPaper}
					className={styles.infoContainer}
					title={"Sensor " + (index + 1)}
				>
					<ValueField label="Temperature" valueEnding="°C" value={sensor.temperature}
								endAdornment={<Img src={temperatureIcon}/>}/>
					<ValueField label="Humidity" valueEnding="%" value={sensor.humidity}
								endAdornment={<Img src={humidityIcon}/>}/>
				</PaperSection>
			))}
			<PaperSection
				paperClassName={styles.infoPaper}
				className={styles.infoContainer}
				title="Led 1"
			>
				<ValueField label="State" value={led1.state ? "ON" : "OFF"}/>
			</PaperSection>
			<PaperSection
				paperClassName={styles.infoPaper}
				className={styles.infoContainer}
				title="Led 2"
			>
				<ValueField label="Effect" value={led2.effects[led2.currentEffect].name.toUpperCase()}/>
			</PaperSection>
		</PaperSection>
	)

}

export default Main;
