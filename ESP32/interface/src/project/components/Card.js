import {Card as MuiCard, CardContent, Typography} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";
import {useEffect, useState} from "react";

const useStyles = makeStyles(() => ({
	card: {
		padding: "1rem",
		margin: "0.5rem",
	},
	content: {
		padding: 0,
		paddingTop: "0.5rem",
		display: "flex",
		flexDirection: "column",
		"&:last-child": {
			paddingBottom: 0
		}
	},
	header: {
		display: "flex",
	}

}));

const Card = ({children, elevation = 5, header, style, className, headerLocation}) => {
	const styles = useStyles();
	const [headerPosition, setHeaderPosition] = useState("center");

	useEffect(() => {
		switch (headerLocation) {
			case "center":
				setHeaderPosition("center");
				break;
			case "left":
				setHeaderPosition("flex-start");
				break;
			case "right":
				setHeaderPosition("flex-end");
				break;
			default:
				setHeaderPosition("center");
				break;
		}
	}, [headerLocation]);

	return (
		<MuiCard className={styles.card} style={style} elevation={elevation}>
			{header ? <Typography variant="h6" className={styles.header}
								  style={{justifyContent: headerPosition}}>{header}</Typography> : null}
			<CardContent className={clsx(styles.content, className)}>
				{children}
			</CardContent>
		</MuiCard>
	)
}

export default Card;