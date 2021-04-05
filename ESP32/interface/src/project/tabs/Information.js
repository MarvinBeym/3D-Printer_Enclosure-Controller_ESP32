import React, {Component} from 'react';
import {SectionContent} from '../../components';
import InfoPanel from "../components/InfoPanel";
import AboutPanel from "../components/AboutPanel";

class Information extends Component {

    render() {
        return (
            <SectionContent title='3D-Printer Enclosure Controller ESP32 Info/About'>
                <InfoPanel/>
                <AboutPanel/>
            </SectionContent>
        )
    }

}

export default Information;
