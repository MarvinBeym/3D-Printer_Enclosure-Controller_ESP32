import PaperSection from "../PaperSection";
import React, {useEffect, useState} from "react";
import {makeStyles} from "@material-ui/core/styles";
import Led2EffectConfigForm from "../form/forms/Led2EffectConfigForm";

const useStyles = makeStyles((theme) => ({
	led2ConfigSection: {
		width: "70%",
		margin: "0",
		padding: "1rem",
		marginLeft: "2rem",
		height: "100%",
	},
	led2ConfigForm: {
		display: "flex",
		flexDirection: "column",
	},
}));

const Led2EffectConfigFormWrapper = ({effectName, effectConfig}) => {
	const styles = useStyles();

	const [show, setShow] = useState(false);
	const [switches, setSwitches] = useState([]);
	const [selects, setSelects] = useState([]);

	useEffect(() => {
		let shouldShow = effectConfig && Object.keys(effectConfig).length > 0;
		setShow(shouldShow);
		if (shouldShow) {
			if (effectConfig.selects.length > 0) {
				setSelects(effectConfig.selects);
			}
			if (effectConfig.switches.length > 0) {
				setSwitches(effectConfig.switches);
			}
		}
	}, [effectConfig, effectConfig?.selects, effectConfig?.switches]);

	if (!show) {
		return null;
	}

	return (
		<PaperSection paperClassName={styles.led2ConfigSection} className={styles.led2ConfigForm}
					  title="Effect configuration">
			<Led2EffectConfigForm effectName={effectName} switches={switches} selects={selects}/>
		</PaperSection>
	)
}

export default Led2EffectConfigFormWrapper;