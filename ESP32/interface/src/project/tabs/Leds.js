import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import {FormControl, InputLabel, MenuItem, Select} from "@material-ui/core";
import {useDispatch, useSelector} from "react-redux";
import {selectLed1, selectLed2,} from "../redux/reducers/ledsSlice";
import {wsSend} from "../redux/reducers/webSocketSlice";
import TabContent from "../components/TabContent";
import Card from "../components/Card";
import SwitchCard from "../components/SwitchCard";
import Led2EffectConfigForm from "../components/form/forms/Led2EffectConfigForm";
import {motion} from "framer-motion";

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
	show: { opacity: 1, y: 0 },
	hide: { opacity: 0, y: "100%" },
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

	const onLed2EffectChange = (event) => {
		dispatch(wsSend({component: "led2", command: "setEffect", value: event.target.value}));
	}

	return (
		<TabContent className={styles.leds}>
			<Card header="Leds">
				<SwitchCard header="Led 1" checked={led1.state} onChange={onLed1Change}/>
				<Card header="Led 2">
					<FormControl fullWidth variant="filled">
						<InputLabel id="demo-simple-select-outlined-label">Effect</InputLabel>
						<Select value={led2.currentEffect} onChange={onLed2EffectChange}
								className={styles.effectChooser}>
							{led2.effects.map((effect) => (
								<MenuItem key={effect.id} value={effect.id}>{effect.name.toUpperCase()}</MenuItem>
							))}
						</Select>
					</FormControl>
				</Card>
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