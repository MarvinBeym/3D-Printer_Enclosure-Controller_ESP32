import React, {useEffect, useState} from 'react';
import {Redirect, Switch, useHistory} from 'react-router-dom'

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
import {loadWebinterfaceConfigFromLocalStorage} from "./redux/reducers/configurationSlice";

store.dispatch(connect(process.env.REACT_APP_WEB_SOCKET_ROOT + "/ws", "ENCLOSURE_CONTROLLER"));

const EnclosureController = ({location}) => {
	const history = useHistory();
	const [defaultPage, setDefaultPage] = useState(store.getState().configuration.webinterface.defaultPage);
	const handleTabChange = (event, path) => {
		history.push(path);
	};


	//Load from localstorage
	useEffect(() => {
		store.dispatch(loadWebinterfaceConfigFromLocalStorage());
	}, []);

	useEffect(() => {
		setDefaultPage(store.getState().configuration.webinterface.defaultPage);
	}, []);

	return (
		<Provider store={store}>
			<MenuAppBar sectionTitle="Enclosure Controller">
				<Tabs value={location.pathname} onChange={handleTabChange} variant="fullWidth">
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
					<Redirect to={`/${PROJECT_PATH}${defaultPage}`}/>
				</Switch>
			</MenuAppBar>
		</Provider>
	)
}

export default EnclosureController;
