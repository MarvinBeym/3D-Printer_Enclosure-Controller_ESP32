import React from 'react';
import Card from "../components/Card";
import TabContent from "../components/TabContent";
import {makeStyles} from "@material-ui/core/styles";
import {Divider, Link, Typography} from "@material-ui/core";

const useStyles = makeStyles((theme) => ({
	information: {
		display: "grid",
		gridTemplateColumns: "80% 20%",
	},
	divider: {
		margin: "0.5rem 0",
		backgroundColor: "black",
	},
}));

const Information = () => {
	const styles = useStyles();
	return (
		<TabContent className={styles.information}>
			<Card header="Information">
				<Typography paragraph>
					This is the webinterface for your enclosure controller.
				</Typography>
				<Typography>
					Values changed on the display or here in the webinterface
				</Typography>
				<Typography>
					can be saved to the ESP32's flash storage by going to the configuration tab.
				</Typography>
				<Typography paragraph>
					The saved values will be loaded from the ESP32's flash storage on boot.
				</Typography>
			</Card>
			<Card header="About & Credits">
				<Card header="Creator">
					<Typography variant="h6">{process.env.REACT_APP_MAKER}</Typography>
				</Card>
				<Card header="Project">
					<Link align="center" href={process.env.REACT_APP_GITHUB_URL}><Typography
						variant="h6">GitHub</Typography></Link>
					<Divider className={styles.divider} variant="fullWidth"/>
					<img alt="GitHub"
						 src="https://img.shields.io/github/license/MarvinBeym/3D-Printer_Enclosure-Controller_ESP32?style=for-the-badge"/>
					<Divider className={styles.divider} variant="fullWidth"/>
					<img alt="GitHub release (latest by date)"
						 src="https://img.shields.io/github/v/release/MarvinBeym/3D-Printer_Enclosure-Controller_ESP32?style=for-the-badge"/>
				</Card>
				<Card header="Credits">
					<Typography variant="h6">Icons: <Link href="https://icons8.com/">Icons8</Link></Typography>
				</Card>
			</Card>
		</TabContent>
	)
}

export default Information;
