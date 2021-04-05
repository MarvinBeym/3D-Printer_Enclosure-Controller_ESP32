import React from 'react';
import {Divider, Grid} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import GridPaper from "./GridPaper";
import PaperSection from "./PaperSection";


const useStyles = makeStyles((theme) => ({
    gridContainer: {
        flexGrow: 1,
    },
    paper: {
        display: "flex",
        flexDirection: "column",
        minHeight: "8rem",
        padding: "1rem",
        maxWidth: "15rem",
        overflowWrap: "break-word",
    },
    divider: {
        margin: "0.5rem 0",
        backgroundColor: theme.palette.primary.main
    }
}));

const AboutPanel = () => {
    const styles = useStyles();

    return (
        <PaperSection title="About" elevation={5}>
            <Grid container justify="center" spacing={2} className={styles.gridContainer}>
                <GridPaper className={styles.paper} title="Creator">{process.env.REACT_APP_MAKER}</GridPaper>
                <GridPaper className={styles.paper} title="Github">
                    <span>Repository: {process.env.REACT_APP_GITHUB_URL}</span>
                    <Divider className={styles.divider} variant="fullWidth"/>
                    <span>Licence: {process.env.REACT_APP_PROJECT_LICENCE}</span>
                </GridPaper>
                <GridPaper className={styles.paper} title="Software versions">
                    <span>ESP32: {process.env.REACT_APP_SOFTWARE_VERSION_ESP32}</span>
                    <Divider className={styles.divider} variant="fullWidth"/>
                    <span>Display: {process.env.REACT_APP_SOFTWARE_VERSION_DISPLAY}</span>
                </GridPaper>
            </Grid>
        </PaperSection>
    );
}

export default AboutPanel;