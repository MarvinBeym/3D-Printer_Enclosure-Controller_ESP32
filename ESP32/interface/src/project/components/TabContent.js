import {makeStyles} from "@material-ui/core/styles";
import clsx from "clsx";


const useStyles = makeStyles((theme) => ({
	tabContent: {
		padding: "1rem",
		display: "flex",
	}
}));

const TabContent = ({className, children}) => {
	const styles = useStyles();
	return (
		<div className={clsx(styles.tabContent, className)}>
			{children}
		</div>
	)
}

export default TabContent;