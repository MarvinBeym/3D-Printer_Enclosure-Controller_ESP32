import React, {useState} from "react";
import {fetchBackend, useInterval} from "../helper";
import {LinearProgress, Typography} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";

const useStyles = makeStyles((theme) => ({
	loading: {
		color: theme.palette.primary.main,
		display: "flex",
		flexDirection: "column",
		justifyContent: "center",
		alignItems: "center",
		width: "100%",
		height: "100%",
		padding: "0 4rem",
	},
	loadingProgress: {
		width: "100%",
		margin: theme.spacing(4),
	},
	loadingDetails: {
		fontSize: "1.8rem",
		fontWeight: "bold",
		textAlign: "center",
	},
	loadingFailure: {
		fontSize: "1.8rem",
		fontWeight: "bold",
	}
}));

const LoadingFailure = ({message = "Fetch error"}) => {
	const styles = useStyles();
	return (
		<div className={clsx(styles.loading, styles.loadingFailure)}>
			{message}
		</div>
	)
}

const Loading = () => {
	const styles = useStyles();
	return (
		<div className={styles.loading}>
			<LinearProgress className={styles.loadingProgress}/>
			<Typography variant="h6" className={styles.loadingDetails}>
				Loading&hellip;
			</Typography>
		</div>
	)
}

const IntervalFetchWrapper = ({
								  fetchEndpoint,
								  render,
								  renderWithoutData = false,
								  fetchInterval = 2000,
								  retries = 5
							  }) => {
	const [fetchData, setFetchData] = useState(null);
	const [tries, setTries] = useState(0);
	useInterval(() => {
		if (fetchEndpoint && fetchEndpoint !== "" && tries !== retries) {
			fetchBackend(fetchEndpoint).then((data) => {
				setFetchData(data);
			}).catch((error) => {
				setTries(tries + 1);
			})
		}
	}, fetchInterval);

	if (fetchData || renderWithoutData) {
		return render(fetchData);
	} else if (tries >= retries) {
		return <LoadingFailure message={"Failed to fetch after " + tries + " retries"}/>
	}

	return <Loading/>

}

export default IntervalFetchWrapper;