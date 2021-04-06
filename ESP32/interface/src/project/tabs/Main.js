import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import PaperSection from "../components/PaperSection";
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";

const useStyles = makeStyles((theme) => ({
	main: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gridTemplateRows: "auto auto",
		gridGap: "1rem",
	},
	infoSection: {
		margin: "0",
	}
}));

const Main = () => {
	const styles = useStyles();
	return (
		<IntervalFetchWrapper fetchInterval={2000} fetchEndpoint="main/data" render={(data) => (
			<PaperSection className={styles.main}>
				<PaperSection paperClassName={styles.infoSection} title="Fan 1">

				</PaperSection>
				<PaperSection paperClassName={styles.infoSection} title="Fan 2">

				</PaperSection>
				<PaperSection paperClassName={styles.infoSection} title="Sensor 1">

				</PaperSection>
				<PaperSection paperClassName={styles.infoSection} title="Sensor 2">

				</PaperSection>
			</PaperSection>
		)}/>
	)

}

export default Main;
