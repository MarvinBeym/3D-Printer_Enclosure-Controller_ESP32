import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	sensor1Data: [],
	sensor2Data: [],
	status: 'idle',
}

export const selectSensor1Data = (state) => state.sensors.sensor1Data;
export const selectSensor2Data = (state) => state.sensors.sensor2Data;
const sensorsSlice = createSlice({
	name: 'sensors',
	initialState,
	reducers: {
		addSensor1Data(state, action) {
			state.sensor1Data = [...state.sensor1Data, action.payload];
		},
		addSensor2Data(state, action) {
			state.sensor2Data = [...state.sensor2Data, action.payload];
		},
		setSensor1Data(state, action) {
			state.sensor1Data = action.payload;
		},
		setSensor2Data(state, action) {
			state.sensor2Data = action.payload;
		},
	},
});

export const {addSensor1Data, addSensor2Data, setSensor1Data, setSensor2Data} = sensorsSlice.actions;

export default sensorsSlice.reducer;
