import React, {useState} from 'react';
import PaperSection from "../components/PaperSection";
import {makeStyles} from "@material-ui/core/styles";
import {FormControl, InputLabel, MenuItem, Select, Switch} from "@material-ui/core";
import {useDispatch, useSelector} from "react-redux";
import {selectLed1State, selectLed2, setLed1State} from "../redux/reducers/ledsSlice";
import {fetchBackend, useInterval} from "../helper";

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
	const led1State = useSelector((state) => selectLed1State(state));
	const led2 = useSelector((state) => selectLed2(state));
	const onLed1Change = (event) => {
		dispatch(setLed1State(event.target.checked));
	}

	const [effects, setEffects] = useState(null);
	const [currentEffect, setCurrentEffect] = useState(0);

	const onLed2EffectChange = (event) => {
		let effectId = event.target.value;
		fetchBackend(`leds/changeEffect/${effectId}`);
		setCurrentEffect(effectId);
	}

	useInterval(() => {
		fetchBackend("leds/data").then((response) => {
			const effectLoader = response.effectLoader;
			if (!effects) {
				setEffects(effectLoader.effects);
			}
			setCurrentEffect(effectLoader.currentEffect);
		});
	}, 2000);

	return (
		<PaperSection className={styles.leds}>
			<PaperSection paperClassName={styles.effectChooserSection} title="Leds">
				<PaperSection title="Led 1" paperClassName={styles.led1Section}>
					<Switch checked={led1State} onChange={onLed1Change}/>
				</PaperSection>
				<PaperSection title="Led 2" paperClassName={styles.ledSection}>
					<FormControl fullWidth variant="filled">
						<InputLabel id="demo-simple-select-outlined-label">Effect</InputLabel>
						<Select value={currentEffect ? currentEffect : 0} onChange={onLed2EffectChange} className={styles.effectChooser}>

							{effects ? effects.map((effect) => (
								<MenuItem key={effect.id} value={effect.id}>{effect.name}</MenuItem>
							)) : <MenuItem value={0}>none</MenuItem>}
						</Select>
					</FormControl>
				</PaperSection>
			</PaperSection>
			{led2?.selectedEffect
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
