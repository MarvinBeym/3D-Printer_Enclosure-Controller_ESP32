import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	config: {
		displayBrightness: 100,
	},
	status: 'idle',
}

export const selectConfiguration = (state) => state.configuration.config;
const configurationSlice = createSlice({
	name: 'configuration',
	initialState,
	reducers: {
		setConfiguration(state, action) {
			let displayBrightness = action.payload.configuration?.displayBrightness;
			if("displayBrightness" in action.payload.configuration) {
				state.config.displayBrightness = displayBrightness;
			}
		}
	},
});

export const {setConfiguration} = configurationSlice.actions;

export default configurationSlice.reducer;
