import React, {useState} from 'react';
import {fetchBackend, useInterval} from "../helper";
import QuickLineChart from "../components/QuickLineChart";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({}));

const Sensors = () => {
    const styles = useStyles();
    const [sensorsData, setSensorsData] = useState(null);
    const [sensor1Temps, setSensor1Temps] = useState([]);
    const [sensor2Temps, setSensor2Temps] = useState([]);
    useInterval(() => {
        fetchBackend("sensors/data").then((response) => {
            let fan1Temp = response.sensor1.temperature;
            let fan2Temp = response.sensor2.temperature;
            let currentTicks = new Date().getTime();

            if (sensor1Temps.length === 0 || sensor1Temps[sensor1Temps.length - 1].temperature !== fan1Temp) {
                setSensor1Temps([...sensor1Temps, {temperature: fan1Temp, time: currentTicks}]);
            }

            if (sensor2Temps.length === 0 || sensor2Temps[sensor2Temps.length - 1].temperature !== fan2Temp) {
                setSensor2Temps([...sensor2Temps, {temperature: fan2Temp, time: currentTicks}]);
            }

            setSensorsData(response);
        })
    }, 2000);

    return (
        <div>
            <div>TTTT</div>
            <QuickLineChart yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time" data={sensor1Temps}
                            dataKey="temperature"/>
            <QuickLineChart yAxisValueSuffix="°C" yAxisLabel="Temperature" xAxisLabel="Time" data={sensor2Temps}
                            dataKey="temperature"/>
        </div>

    )
}

export default Sensors;
