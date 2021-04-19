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

	if (!(effectConfig && Object.keys(effectConfig).length > 0)) {
		return null;
	}

	return (
		<PaperSection paperClassName={styles.led2ConfigSection} className={styles.led2ConfigForm}
					  title="Effect configuration">
			<Led2EffectConfigForm effectName={effectName} configFields={effectConfig}/>
		</PaperSection>
	)
}

export default Led2EffectConfigFormWrapper;