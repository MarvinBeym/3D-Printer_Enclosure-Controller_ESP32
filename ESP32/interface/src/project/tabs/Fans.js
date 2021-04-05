import React from 'react';
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";
import PaperSection from "../components/PaperSection";
import {makeStyles} from "@material-ui/core/styles";
import {motion} from "framer-motion";
import fanImage from "../images/fan.png";
import {Slider} from "@material-ui/core";
import ValueField from "../components/ValueField";

const useStyles = makeStyles((theme) => ({
	fanPaper: {
		display: "flex",

	},
	fanInfo: {
		display: "flex",
		flexDirection: "column",
		padding: "2rem 0",
		marginRight: "2rem",
		marginLeft: "2rem",
		minWidth: "12rem",
		minHeight: "12rem",
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

		width: "12rem",
		height: "12rem",
	},
	fanImage: {
		width: "100%",
		height: "100%",
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
						<PaperSection key={index} className={styles.fanPaper} title={"Fan " + index + 1}>
							<div className={styles.fanWrapper}>
								<motion.div className={styles.fanImageWrapper} animate={{rotate: 360}}
											transition={spinTransition}>
									<img className={styles.fanImage} src={fanImage} alt="fan image"/>
								</motion.div>
							</div>
							<div className={styles.fanInfo}>
								<Slider min={0} max={100} step={1} marks={sliderMarks} valueLabelDisplay="auto"
										defaultValue={fan[1].percent}/>
								<div style={{display: "flex", justifyContent: "space-evenly"}}>
									<ValueField label="Rpm" value={fan[1].rpm}/>
									<ValueField label="Duty cycle" valueSuffix="%" value={fan[1].dutyCycle}/>
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
