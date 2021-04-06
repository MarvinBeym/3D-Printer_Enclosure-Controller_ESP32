import React from "react";
import clsx from 'clsx';
import {Paper, Typography} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	paper: {
		padding: theme.spacing(2),
		margin: theme.spacing(3),
	},
	header: {
		display: "flex",
		marginBottom: "1rem",
	}
}));

const PaperSection = ({paperClassName, className, title, headerJustification = "center", elevation = 5, children}) => {
	const styles = useStyles();

	return (
		<Paper elevation={elevation} className={clsx(styles.paper, paperClassName)}>
			<Typography className={styles.header} styles={{justifyContent: headerJustification}}
						variant="h6">{title}</Typography>
			<div className={className}>
				{children}
			</div>
		</Paper>
	)
}

export default PaperSection;