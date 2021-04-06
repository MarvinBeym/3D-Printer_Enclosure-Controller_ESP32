import React from 'react';
import PaperSection from "../components/PaperSection";
import {makeStyles} from "@material-ui/core/styles";
import {FormControl, InputLabel, MenuItem, Select, Switch} from "@material-ui/core";
import {useDispatch, useSelector} from "react-redux";
import {selectLed1State, selectLed2, setLed1State} from "../redux/reducers/ledsSlice";

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

	return (
		<PaperSection className={styles.leds}>
			<PaperSection paperClassName={styles.effectChooserSection} title="Leds">
				<PaperSection title="Led 1" paperClassName={styles.led1Section}>
					<Switch checked={led1State} onChange={onLed1Change}/>
				</PaperSection>
				<PaperSection title="Led 2" paperClassName={styles.ledSection}>
					<FormControl fullWidth variant="filled">
						<InputLabel id="demo-simple-select-outlined-label">Effect</InputLabel>
						<Select className={styles.effectChooser}>
							<MenuItem value={10}>Ten</MenuItem>
							<MenuItem value={20}>Twenty</MenuItem>
							<MenuItem value={30}>Thirty</MenuItem>
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
