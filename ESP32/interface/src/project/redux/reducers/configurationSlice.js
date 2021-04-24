import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	config: {
		displayBrightness: 100,
	},
	webinterface: {
		fanSpinAnimation: true,
		defaultPage: "/enclosure-controller/information",
		showSensorGraph: {
			sensor1: true,
			sensor2: false,
		},
	},
	status: 'idle',
}

export const selectConfiguration = (state) => state.configuration.config;
export const selectWebinterfaceConfig = (state) => state.configuration.webinterface;
const configurationSlice = createSlice({
	name: 'configuration',
	initialState,
	reducers: {
		setConfiguration(state, action) {
			let displayBrightness = action.payload.configuration?.displayBrightness;
			if("displayBrightness" in action.payload.configuration) {
				state.config.displayBrightness = displayBrightness;
			}
		},
		setFanSpinAnimation(state, action) {
			state.webinterface.fanSpinAnimation = action.payload;
			saveWebinterfaceConfigToLocalStorage(state.webinterface);
		},
		setDefaultPage(state, action) {
			state.webinterface.defaultPage = action.payload;
			saveWebinterfaceConfigToLocalStorage(state.webinterface);
		},
		setShowSensor1Graph(state, action) {
			state.webinterface.showSensorGraph.sensor1 = action.payload;
			saveWebinterfaceConfigToLocalStorage(state.webinterface);
		},
		setShowSensor2Graph(state, action) {
			state.webinterface.showSensorGraph.sensor2 = action.payload;
			saveWebinterfaceConfigToLocalStorage(state.webinterface);
		},
		loadWebinterfaceConfigFromLocalStorage(state) {
			let jsonString = localStorage.getItem("webinterface");
			if(!jsonString) {
				return;
			}

			let json = JSON.parse(jsonString);
			if(!json) {
				return;
			}

			Object.keys(json).forEach((key) => {
				if(!(key in state.webinterface) || !(typeof(state.webinterface[key]) === typeof(json[key]))) {
					//Skip value invalid;
					return;
				}

				state.webinterface[key] = json[key];
			})
		}
	},
});

const saveWebinterfaceConfigToLocalStorage = (state) => {
	let jsonString = JSON.stringify(state);
	localStorage.setItem("webinterface", jsonString);
}

export const {setConfiguration, setDefaultPage, setFanSpinAnimation, loadWebinterfaceConfigFromLocalStorage, setShowSensor1Graph, setShowSensor2Graph} = configurationSlice.actions;

export default configurationSlice.reducer;
