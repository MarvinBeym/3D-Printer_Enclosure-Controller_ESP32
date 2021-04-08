import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	sensor1: {
		temperature: 0,
		humidity: 0,
		temperatureCollection: [],
		humidityCollection: [],
	},
	sensor2: {
		temperature: 0,
		humidity: 0,
		temperatureCollection: [],
		humidityCollection: [],
	},
	status: 'idle',
}

export const selectSensor1 = (state) => state.sensors.sensor1;
export const selectSensor2 = (state) => state.sensors.sensor2;
const sensorsSlice = createSlice({
	name: 'sensors',
	initialState,
	reducers: {
		setSensor1(state, action) {
			let temperature = action.payload.sensor1?.temperature;
			let humidity = action.payload.sensor1?.humidity;
			if(temperature) {
				state.sensor1.temperature = temperature;
				state.sensor1.temperatureCollection = [...state.sensor1.temperatureCollection, temperature]
			}
			if(humidity) {
				state.sensor1.humidity = humidity;
				state.sensor1.humidityCollection = [...state.sensor1.humidityCollection, humidity]
			}
		},
		setSensor2(state, action) {
			let temperature = action.payload.sensor2?.temperature;
			let humidity = action.payload.sensor2?.humidity;
			if(temperature) {
				state.sensor2.temperature = temperature;
				state.sensor2.temperatureCollection = [...state.sensor2.temperatureCollection, temperature]
			}
			if(humidity) {
				state.sensor2.humidity = humidity;
				state.sensor2.humidityCollection = [...state.sensor2.humidityCollection, humidity]
			}
		},
	},
});

export const {setSensor1, setSensor2} = sensorsSlice.actions;

export default sensorsSlice.reducer;
