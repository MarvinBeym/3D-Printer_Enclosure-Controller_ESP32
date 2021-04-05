import React, {useState} from 'react';
import {fetchBackend, useInterval} from "../helper";

// @ts-ignore
const Sensors = () => {
    const [sensorsData, setSensorsData] = useState(null);
    useInterval(() => {
        fetchBackend("sensors/data").then((response) => {
            setSensorsData(response);
        })
    }, 2000);

    return (
        <div>TTTT</div>
    )
}

export default Sensors;
