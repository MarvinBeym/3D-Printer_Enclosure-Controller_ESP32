import React, {useEffect} from 'react';
import PaperSection from "../components/PaperSection";
import {makeStyles} from "@material-ui/core/styles";
import {FormControl, InputLabel, MenuItem, Select, Switch} from "@material-ui/core";
import {useDispatch, useSelector} from "react-redux";
import {
	changeLed2Effect,
	changeLed1State, fetchCurrentLed2Effect,
	fetchLed2Effects, fetchLed1State,
	selectLed1,
	selectLed2,
} from "../redux/reducers/ledsSlice";
import {useInterval} from "../helper";

const useStyles = makeStyles((theme) => ({
	leds: {
		display: "flex",
		width: "100%",
	},
	effectChooserSection: {
		width: "30%",
	},
	effectChooser: {
		width: "100%",
	},
	led2ConfigSection: {
		flex: "1",
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

const Leds = () => {
	const styles = useStyles();
	const dispatch = useDispatch();

	const led1 = useSelector((state) => selectLed1(state));
	const led2 = useSelector((state) => selectLed2(state));
	const onLed1Change = (event) => {
		dispatch(changeLed1State(event.target.checked));
	}

	const onLed2EffectChange = (event) => {
		dispatch(changeLed2Effect(event.target.value));
	}

	useEffect(() => {
		dispatch(fetchLed2Effects());
	}, [dispatch]);
	
	useInterval(() => {
		dispatch(fetchLed1State());
		dispatch(fetchCurrentLed2Effect());
	}, 2000);

	return (
		<PaperSection className={styles.leds}>
			<PaperSection paperClassName={styles.effectChooserSection} title="Leds">
				<PaperSection title="Led 1" paperClassName={styles.led1Section}>
					<Switch checked={led1.currentState} onChange={onLed1Change}/>
				</PaperSection>
				<PaperSection title="Led 2" paperClassName={styles.ledSection}>
					<FormControl fullWidth variant="filled">
						<InputLabel id="demo-simple-select-outlined-label">Effect</InputLabel>
						<Select value={led2.currentEffect ? led2.currentEffect : 1} onChange={onLed2EffectChange} className={styles.effectChooser}>
							{led2.effects.map((effect) => (
								<MenuItem key={effect.id} value={effect.id}>{effect.name}</MenuItem>
							))}
						</Select>
					</FormControl>
				</PaperSection>
			</PaperSection>
			{led2.effects[led2.currentEffect]?.configuration
				? (
					<PaperSection paperClassName={styles.led2ConfigSection} title="Led 2 Configuration">
						ZZZ
					</PaperSection>
				) : null
			}
		</PaperSection>
	)
}

export default Leds;
