import Card from "./Card";
import {Switch} from "@material-ui/core";
import React from "react";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	switchCard: {
		alignItems: "center",
	}
}));

const SwitchCard = ({checked, onChange, header}) => {
	const styles = useStyles();

	const onSwitchChange = (event) => {
		onChange(event.target.checked);
	}

	return (
		<Card className={styles.switchCard} header={header}>
			<Switch checked={checked} onChange={onSwitchChange}/>
		</Card>
	)
}

export default SwitchCard;