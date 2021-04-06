import React from 'react';
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";
import PaperSection from "../components/PaperSection";
import {makeStyles} from "@material-ui/core/styles";
import {motion} from "framer-motion";
import fanImage from "../images/fan.png";
import speedIcon from "../images/speed_icon_x32.png";
import percentIcon from "../images/percent_icon_x32.png";
import {Slider} from "@material-ui/core";
import ValueField from "../components/ValueField";
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

const Fans = () => {
	const styles = useStyles();

	const sliderMarks = [
		{value: 0, label: "0%"},
		{value: 25, label: "25%"},
		{value: 50, label: "50%"},
		{value: 75, label: "75%"},
		{value: 100, label: "100%"},
	];

	return <IntervalFetchWrapper fetchInterval={500} fetchEndpoint="fans/data" render={(data) => (
		<div className={styles.fans}>
			{data
				? Object.entries(data).map((fan, index) => {
					return (
						<PaperSection key={index} className={styles.fanPaper} title={"Fan " + (index + 1)}>
							<div className={styles.fanWrapper}>
								<motion.div className={styles.fanImageWrapper} animate={{rotate: 360}}
											transition={spinTransition}>
									<img className={styles.fanImage} src={fanImage} alt=""/>
								</motion.div>
							</div>
							<div className={styles.fanInfo}>
								<Slider min={0} max={100} step={1} marks={sliderMarks} valueLabelDisplay="auto"
										defaultValue={fan[1].percent}/>
								<div className={styles.valueFieldsContainer}>
									<ValueField
										endAdornment={<Img src={speedIcon}/>}
										label="Rpm"
										value={fan[1].rpm}
									/>
									<ValueField
										endAdornment={<Img src={percentIcon}/>}
										label="Duty cycle"
										value={fan[1].dutyCycle}
									/>
								</div>
							</div>
						</PaperSection>
					)
				})
				: null
			}
		</div>
	)}/>

}

export default Fans;
