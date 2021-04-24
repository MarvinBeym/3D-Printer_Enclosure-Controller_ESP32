import Card from "./Card";
import {IconButton} from "@material-ui/core";
import saveIcon from "../images/save_x64.png";
import Img from "./Img";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	iconButtonCard: {
		width: "fit-content",
		height: "fit-content",
	},
	cardContent: {
		padding: 0,
	},
	iconButton: {
		padding: 0,
		"&:hover": {
			//you want this to be the same as the backgroundColor above
			backgroundColor: "transparent"
		}
	},
	iconImage: {
		padding: "0.1rem",
		"&:hover": {
			//you want this to be the same as the backgroundColor above
			backgroundColor: "rgb(239, 239, 239)",

			borderRadius: "1rem",
		}
	}
}));

const IconButtonCard = ({onClick, header}) => {
	const styles = useStyles();
	return (
		<Card cardClassName={styles.iconButtonCard} className={styles.cardContent} header={header}>
			<IconButton className={styles.iconButton} onClick={onClick}>
				<Img className={styles.iconImage} src={saveIcon}/>
			</IconButton>
		</Card>
	)
}

export default IconButtonCard;