import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import PaperSection from "../components/PaperSection";
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";
import ValueField from "../components/ValueField";
import temperatureIcon from "../images/temp_icon_x32.png";
import humidityIcon from "../images/humidity_icon_x32.png";
import speedIcon from "../images/speed_icon_x32.png";
import percentIcon from "../images/percent_icon_x32.png";
import Img from "../components/Img";


const useStyles = makeStyles((theme) => ({
	main: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gridTemplateRows: "auto auto",
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
	return (
		<IntervalFetchWrapper fetchInterval={2000} fetchEndpoint="main/data" render={(data) => (
			<PaperSection className={styles.main}>
				{[data.fan1, data.fan2].map((fan, index) => (
					<PaperSection
						paperClassName={styles.infoPaper}
						className={styles.infoContainer}
						title={"Fan " + (index + 1)}
					>
						<ValueField label="Rpm" value={fan.rpm} endAdornment={<Img src={speedIcon}/>}/>
						<ValueField label="Percent" value={fan.percent} endAdornment={<Img src={percentIcon}/>}/>
					</PaperSection>
				))}
				{[data.sensor1, data.sensor2].map((sensor, index) => (
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
			</PaperSection>
		)}/>
	)

}

export default Main;
