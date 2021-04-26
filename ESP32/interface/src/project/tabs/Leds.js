import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import {selectLed1, selectLed2,} from "../redux/reducers/ledsSlice";
import {wsSend} from "../redux/reducers/webSocketSlice";
import TabContent from "../components/TabContent";
import Card from "../components/Card";
import SwitchCard from "../components/SwitchCard";
import Led2EffectConfigForm from "../components/form/forms/Led2EffectConfigForm";
import {motion} from "framer-motion";
import SelectCard from "../components/SelectCard";

const useStyles = makeStyles((theme) => ({
	leds: {
		display: "grid",
		gridTemplateColumns: "25% 1fr",
	},
	effectChooserSection: {
		margin: "0",
		padding: "1rem",
		width: "30%",
	},
	effectChooser: {
		width: "100%",
	},
	led1Section: {
		display: "flex",
		flexDirection: "column",
		alignItems: "center",
		width: "100%",
		margin: "0",
		marginBottom: "1rem",
	},
	ledSection: {
		display: "flex",
		flexDirection: "column",
		margin: "0",
		marginTop: "1rem",
	},
}));

const variants = {
	show: {opacity: 1, y: 0},
	hide: {opacity: 0, y: "100%"},
}

const Leds = () => {
	const styles = useStyles();
	const dispatch = useDispatch();

	const led1 = useSelector((state) => selectLed1(state));
	const led2 = useSelector((state) => selectLed2(state));
	const currentEffect = led2.effects[led2.currentEffect];
	const currentEffectHasConfig = currentEffect.config && Object.keys(currentEffect.config).length > 0;

	const onLed1Change = (checked) => {
		dispatch(wsSend({component: "led1", command: "setState", value: checked ? 1 : 0}));
	}

	const onLed2EffectChange = (value) => {
		dispatch(wsSend({component: "led2", command: "setEffect", value: value}));
	}

	return (
		<TabContent className={styles.leds}>
			<Card header="Leds">
				<SwitchCard header="Led 1 State" checked={led1.state} onChange={onLed1Change}/>
				<SelectCard
					header="Led2 Effect"
					value={led2.currentEffect}
					onChange={onLed2EffectChange}
					options={led2.effects.map((effect) => {
						return {value: effect.id, label: effect.name.toUpperCase()}
					})}
				/>
			</Card>
			<motion.div animate={currentEffectHasConfig ? "show" : "hide"} variants={variants}>
				<Card header="Effect configuration">
					{currentEffectHasConfig ?
						<Led2EffectConfigForm effect={currentEffect}/>
						: null}
				</Card>
			</motion.div>
		</TabContent>

	)
}

export default Leds;