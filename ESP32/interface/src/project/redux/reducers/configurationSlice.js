import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	config: {
		displayBrightness: 100,
	},
	webinterface: {
		defaultPage: "/enclosure-controller/information",
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
		setDefaultPage(state, action) {
			state.webinterface.defaultPage = action.payload;
		}
	},
});

export const {setConfiguration, setDefaultPage} = configurationSlice.actions;

export default configurationSlice.reducer;
