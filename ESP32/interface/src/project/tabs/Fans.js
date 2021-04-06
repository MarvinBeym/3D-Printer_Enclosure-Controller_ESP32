import React, {useState} from 'react';
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";
import {makeStyles} from "@material-ui/core/styles";
import {authorizedFetch} from "../../authentication";
import {ENDPOINT_ROOT} from "../../api";
import PaperSection from "../components/PaperSection";
import {Slider} from "@material-ui/core";
import ValueField from "../components/ValueField";
import {motion} from "framer-motion";
import speedIcon from "../images/speed_icon_x32.png";
import percentIcon from "../images/percent_icon_x32.png";
import fanImage from "../images/fan.png";
import Img from "../components/Img";

const useStyles = makeStyles((theme) => ({
	fanPaper: {
		display: "flex",
		alignItems: "center",
	},
	fanInfo: {
		display: "flex",
		flexDirection: "column",
		height: "60%",
		marginRight: "2rem",
		marginLeft: "2rem",
		justifyContent: "space-between",
	},
	fans: {
		display: "flex",
	},
	fanWrapper: {
		padding: "0.3rem",
		border: `solid 0.3rem ${theme.palette.primary.main}`,
		borderRadius: "3rem",
	},
	fanImageWrapper: {

		width: "8rem",
		height: "8rem",
	},
	fanImage: {
		width: "100%",
		height: "100%",
	},
	valueFieldsContainer: {
		display: "flex",
		paddingTop: "1rem",
	}
}));

const spinTransition = {
	repeat: Infinity,
	ease: "linear",
	duration: 1
};

const onSliderCommitted = (fanName, newPercent) => {
	switch (fanName) {
		case "fan1":
		case "fan2":
			const data = {};
			data[fanName] = newPercent;
			authorizedFetch(ENDPOINT_ROOT + "fans/setSpeed", {
				method: "POST",
				body: JSON.stringify(data),
			})
			break;
	}


}

const Fans = () => {
	const styles = useStyles();

	const sliderMarks = [
		{value: 0, label: "0%"},
		{value: 25, label: "25%"},
		{value: 50, label: "50%"},
		{value: 75, label: "75%"},
		{value: 100, label: "100%"},
	];

	const [sliderDefaultValues, setSliderDefaultValues] = useState({fan1: -1, fan2: -1});

	return <IntervalFetchWrapper fetchInterval={500} fetchEndpoint="fans/data" render={(data) => {
		const entries = Object.entries(data);
		const fans = [entries[0][1], entries[1][1]];

		if (sliderDefaultValues.fan1 === -1) {
			sliderDefaultValues.fan1 = fans[0].percent;
		}
		if (sliderDefaultValues.fan2 === -1) {
			sliderDefaultValues.fan2 = fans[1].percent;
		}

		return (
			(
				<div className={styles.fans}>
					{fans.map((fan, index) => (
						<PaperSection key={index} className={styles.fanPaper} title={"Fan " + (index + 1)}>
							<div className={styles.fanWrapper}>
								<motion.div className={styles.fanImageWrapper} animate={{rotate: 360}}
											transition={spinTransition}>
									<img className={styles.fanImage} src={fanImage} alt=""/>
								</motion.div>
							</div>
							<div className={styles.fanInfo}>
								<Slider min={0} max={100} step={1}
										onChangeCommitted={(event, value) => onSliderCommitted("fan" + (index + 1), value)}
										marks={sliderMarks} valueLabelDisplay="auto"
										defaultValue={sliderDefaultValues["fan" + (index + 1)]}/>
								<div className={styles.valueFieldsContainer}>
									<ValueField
										endAdornment={<Img src={speedIcon}/>}
										label="Rpm"
										value={fan.rpm}
									/>
									<ValueField
										endAdornment={<Img src={percentIcon}/>}
										label="Duty cycle"
										value={fan.dutyCycle}
									/>
								</div>
							</div>
						</PaperSection>
					))}
				</div>
			)
		)
	}}/>

}

export default Fans;
