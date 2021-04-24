import React, {useEffect, useState} from 'react';
import {makeStyles} from "@material-ui/core/styles";
import {Slider} from "@material-ui/core";
import ValueField from "../components/ValueField";
import {motion} from "framer-motion";
import speedIcon from "../images/speed_icon_x32.png";
import fanImage from "../images/fan.png";
import Img from "../components/Img";
import {useDispatch, useSelector} from "react-redux";
import {selectFan1, selectFan2, selectFans} from "../redux/reducers/fansSlice";
import {wsSend} from "../redux/reducers/webSocketSlice";
import Card from "../components/Card";
import TabContent from "../components/TabContent";
import {selectWebinterfaceConfig} from "../redux/reducers/configurationSlice";

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
		display: "grid",
		gridTemplateColumns: "auto auto",
		paddingTop: "1rem",
		gap: "1rem",
	},
	fanCard: {
		display: "flex",
		flexDirection: "row",
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
	const fans = useSelector((state) => selectFans(state));
	const [fan1SliderValue, setFan1SliderValue] = useState(fan1.percent);
	const [fan2SliderValue, setFan2SliderValue] = useState(fan2.percent);

	const webinterfaceConfiguration = useSelector((state) => selectWebinterfaceConfig(state));

	const sliderMarks = [
		{value: 0, label: "0%"},
		{value: 25, label: "25%"},
		{value: 50, label: "50%"},
		{value: 75, label: "75%"},
		{value: 100, label: "100%"},
	];

	const onFanSliderChange = (fanName, newPercent) => {
		if (fanName === "fan1") {
			setFan1SliderValue(newPercent);
		}
		if (fanName === "fan2") {
			setFan2SliderValue(newPercent);
		}
	}

	const onFanSliderCommitted = (fanName, newPercent) => {
		dispatch(wsSend({component: fanName, command: "setPercent", value: newPercent}));
	}

	useEffect(() => {
		setFan1SliderValue(fan1.percent);
	}, [fan1]);

	useEffect(() => {
		setFan2SliderValue(fan2.percent);
	}, [fan2]);

	return (
		<TabContent>
			{fans.map((fan, index) => {
				const fanName = "fan" + (index + 1);
				const slideValue = fanName === "fan1" ? fan1SliderValue : fan2SliderValue;
				return (
					<Card key={fanName} header={"Fan " + (index + 1)} className={styles.fanCard}>
						<div className={styles.fanWrapper}>
							<motion.div className={styles.fanImageWrapper} animate={{rotate: webinterfaceConfiguration.fanSpinAnimation ? 360 : 0}}
										transition={spinTransition}>
								<img className={styles.fanImage} src={fanImage} alt=""/>
							</motion.div>
						</div>
						<div className={styles.fanInfo}>
							<Slider min={0} max={100} step={1}
									onChange={(event, value) => onFanSliderChange(fanName, value)}
									onChangeCommitted={(event, value) => onFanSliderCommitted(fanName, value)}
									marks={sliderMarks} valueLabelDisplay="auto"
									value={slideValue}/>
							<div className={styles.valueFieldsContainer}>
								<ValueField
									endAdornment={<Img src={speedIcon}/>}
									label="Rpm"
									value={fan.rpm}
								/>
								<ValueField
									label="Duty cycle"
									value={fan.dutyCycle}
								/>
							</div>
						</div>
					</Card>
				)
			})}
		</TabContent>
	)

}

export default Fans;
