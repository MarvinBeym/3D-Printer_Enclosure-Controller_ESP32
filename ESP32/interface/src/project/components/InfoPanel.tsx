import React, {Component} from 'react';
import {Typography} from "@material-ui/core";
import PaperSection from "./PaperSection";

class InfoPanel extends Component {
    render() {
        return (
            <PaperSection title="Info" elevation={5}>
                <Typography variant="body1" paragraph>
                    <Typography>
                        This is the webinterface for your Enclosure Controller based on the ESP32.
                    </Typography>
                    <Typography>
                        At the top of the screen you can find multiple tabs.
                    </Typography>
                </Typography>

                <Typography variant="body1" paragraph>
                    <Typography>
                        On the "Fans" page for example you can see the information about your connected fans.
                    </Typography>
                    <Typography>
                        You can also change the fan speed.
                    </Typography>
                </Typography>

                <Typography variant="body1" paragraph>
                    <Typography>
                        Values like the fan speed can also be saved to the ESP32's file system.
                    </Typography>
                    <Typography>
                        They will then be loaded from the file system again when the esp32 reboots.
                    </Typography>
                </Typography>

                <Typography variant="body1">
                    <Typography>
                        To reduce wear on the flash chip values are not saved every time you change them.
                    </Typography>
                    <Typography>
                        You need to go to the "Configuration" tab and hit the "Save to Flash" button.
                    </Typography>
                </Typography>
            </PaperSection>
        );
    }
}

export default InfoPanel;