import React from "react";
import {Grid, Paper, Typography} from "@material-ui/core";


// @ts-ignore
const GridPaper = ({className, children, title}) => {
    return (
        <Grid item>
            <Paper elevation={5} className={className}>
                <Typography variant="h6" style={{paddingBottom: "0.5rem", alignSelf: "center"}}>{title}</Typography>
                {children}
            </Paper>
        </Grid>
    )
}

export default GridPaper;