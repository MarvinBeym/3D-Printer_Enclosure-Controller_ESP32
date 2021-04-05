import React from 'react';
import IntervalFetchWrapper from "../components/IntervalFetchWrapper";

const Fans = () => {
    // @ts-ignore
    return <IntervalFetchWrapper fetchEndpoint="fans/data" render={(data) => (
        <div>TTT</div>
    )}/>

}

export default Fans;
