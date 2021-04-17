import PaperSection from "../PaperSection";
import React from "react";
import {makeStyles} from "@material-ui/core/styles";
import SolidEffectForm from "./effectForms/SolidEffectForm";

const useStyles = makeStyles((theme) => ({
	led2ConfigSection: {
		flex: "1",
	},
	led2ConfigForm: {
		display: "flex",
		flexDirection: "column",
	},
}));

const Led2EffectConfigFormWrapper = ({effect}) => {
	const styles = useStyles();
	let showConfigForm = Object.keys(effect?.configSetup).length > 0;

	let EffectForm;

	switch (effect.name) {
		case "solid":
			EffectForm = SolidEffectForm;
			break;
		default:
			EffectForm = null;
			break;
	}

	if (!showConfigForm) {
		return null;
	}

	return (
		<PaperSection paperClassName={styles.led2ConfigSection} className={styles.led2ConfigForm}
					  title="Effect configuration">
			<EffectForm configSetup={effect.configSetup}/>
		</PaperSection>
	)
}

export default Led2EffectConfigFormWrapper;