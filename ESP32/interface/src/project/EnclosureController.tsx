import React, {Component} from 'react';
import {Redirect, RouteComponentProps, Switch} from 'react-router-dom'

import {Tab, Tabs} from '@material-ui/core';

import {PROJECT_PATH} from '../api';
import {MenuAppBar} from '../components';
import {AuthenticatedRoute} from '../authentication';

import Information from './tabs/Information';
import Main from './tabs/Main';
import Fans from './tabs/Fans';
import Sensors from './tabs/Sensors';
import Leds from './tabs/Leds';
import Configuration from './tabs/Configuration';
import {Provider} from "react-redux";
import store from './redux/store';
import {connect} from "@giantmachines/redux-websocket";

store.dispatch(connect(process.env.REACT_APP_WEB_SOCKET_ROOT + "/ws", "ENCLOSURE_CONTROLLER"));

class EnclosureController extends Component<RouteComponentProps> {
    handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
        this.props.history.push(path);
    };

    render() {
        return (
            <Provider store={store}>
                <MenuAppBar sectionTitle="Enclosure Controller">
                    <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/information`} label="Information"/>
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/main`} label="Main"/>
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/fans`} label="Fans"/>
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/sensors`} label="Sensors"/>
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/leds`} label="Leds"/>
                        <Tab value={`/${PROJECT_PATH}/enclosure-controller/configuration`} label="Configuration"/>
                    </Tabs>
                    <Switch>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/information`}
                                            component={Information}/>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/main`} component={Main}/>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/fans`} component={Fans}/>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/sensors`}
                                            component={Sensors}/>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/leds`} component={Leds}/>
                        <AuthenticatedRoute exact path={`/${PROJECT_PATH}/enclosure-controller/configuration`}
                                            component={Configuration}/>
                        <Redirect to={`/${PROJECT_PATH}/enclosure-controller/information`}/>
                    </Switch>
                </MenuAppBar>
            </Provider>

        )
    }

}

export default EnclosureController;
