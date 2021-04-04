import React from "react";
import {Paper, Typography} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	paper: {
		padding: theme.spacing(2),
		margin: theme.spacing(3),
	}
}));

const PaperSection = ({title, elevation = 3, children}) => {
	const styles = useStyles();

	return (
		<Paper elevation={elevation} className={styles.paper}>
			<Typography variant="h6">{title}</Typography>
			{children}
		</Paper>
	)
}

export default PaperSection;