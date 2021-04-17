import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import PaperSection from "../components/PaperSection";
import {Slider} from "@material-ui/core";
import ValueField from "../components/ValueField";
import {motion} from "framer-motion";
import speedIcon from "../images/speed_icon_x32.png";
import fanImage from "../images/fan.png";
import Img from "../components/Img";
import {useDispatch, useSelector} from "react-redux";
import {selectFan1, selectFan2} from "../redux/reducers/fansSlice";
import {wsSend} from "../redux/reducers/webSocketSlice";

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
	const dispatch = useDispatch();
	const fan1 = useSelector((state) => selectFan1(state));
	const fan2 = useSelector((state) => selectFan2(state));
	const [fan1SliderValue, setFan1SliderValue] = useState(fan1.percent);
	const [fan2SliderValue, setFan2SliderValue] = useState(fan2.percent);

	const sliderMarks = [
		{value: 0, label: "0%"},
		{value: 25, label: "25%"},
		{value: 50, label: "50%"},
		{value: 75, label: "75%"},
		{value: 100, label: "100%"},
	];

	const onFan1SliderChange = (newPercent) => {
		setFan1SliderValue(newPercent);
	}
	const onFan2SliderChange = (newPercent) => {
		setFan2SliderValue(newPercent);
	}

	const onFan1SliderCommitted = (newPercent) => {
		dispatch(wsSend({component: "fan1", command: "setPercent", value: newPercent}));
	}

	const onFan2SliderCommitted = (newPercent) => {
		dispatch(wsSend({component: "fan2", command: "setPercent", value: newPercent}));
	}

	useEffect(() => {
		setFan1SliderValue(fan1.percent);
	}, [fan1]);

	useEffect(() => {
		setFan2SliderValue(fan2.percent);
	}, [fan2]);

	return (
		<div className={styles.fans}>
			<PaperSection className={styles.fanPaper} title="Fan 1">
				<div className={styles.fanWrapper}>
					<motion.div className={styles.fanImageWrapper} animate={{rotate: 360}}
								transition={spinTransition}>
						<img className={styles.fanImage} src={fanImage} alt=""/>
					</motion.div>
				</div>
				<div className={styles.fanInfo}>
					<Slider min={0} max={100} step={1}
							onChange={(event, value) => onFan1SliderChange(value)}
							onChangeCommitted={(event, value) => onFan1SliderCommitted(value)}
							marks={sliderMarks} valueLabelDisplay="auto"
							value={fan1SliderValue}/>
					<div className={styles.valueFieldsContainer}>
						<ValueField
							endAdornment={<Img src={speedIcon}/>}
							label="Rpm"
							value={fan1.rpm}
						/>
						<ValueField
							label="Duty cycle"
							value={fan1.dutyCycle}
						/>
					</div>
				</div>
			</PaperSection>
			<PaperSection className={styles.fanPaper} title="Fan 2">
				<div className={styles.fanWrapper}>
					<motion.div className={styles.fanImageWrapper} animate={{rotate: 360}}
								transition={spinTransition}>
						<img className={styles.fanImage} src={fanImage} alt=""/>
					</motion.div>
				</div>
				<div className={styles.fanInfo}>
					<Slider min={0} max={100} step={1}
							onChange={(event, value) => onFan2SliderChange(value)}
							onChangeCommitted={(event, value) => onFan2SliderCommitted(value)}
							marks={sliderMarks} valueLabelDisplay="auto"
							value={fan2SliderValue}/>
					<div className={styles.valueFieldsContainer}>
						<ValueField
							endAdornment={<Img src={speedIcon}/>}
							label="Rpm"
							value={fan2.rpm}
						/>
						<ValueField
							label="Duty cycle"
							value={fan2.dutyCycle}
						/>
					</div>
				</div>
			</PaperSection>
		</div>
	)

}

export default Fans;
