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
export const selectSensors = (state) => [state.sensors.sensor1, state.sensors.sensor2];
const sensorsSlice = createSlice({
	name: 'sensors',
	initialState,
	reducers: {
		setSensor1(state, action) {
			let temperature = action.payload.sensor1?.temperature;
			let humidity = action.payload.sensor1?.humidity;
			if("temperature" in action.payload.sensor1) {
				state.sensor1.temperature = temperature;
				const temperatureData = {temperature: temperature, time: new Date().getTime()};
				state.sensor1.temperatureCollection = [...state.sensor1.temperatureCollection, temperatureData];
			}
			if("humidity" in action.payload.sensor1) {
				state.sensor1.humidity = humidity;
				state.sensor1.humidityCollection = [...state.sensor1.humidityCollection, humidity]
				const humidityData = {humidity: humidity, time: new Date().getTime()};
				state.sensor1.humidityCollection = [...state.sensor1.humidityCollection, humidityData];
			}
		},
		setSensor2(state, action) {
			let temperature = action.payload.sensor2?.temperature;
			let humidity = action.payload.sensor2?.humidity;
			if("temperature" in action.payload.sensor2) {
				state.sensor2.temperature = temperature;
				const temperatureData = {temperature: temperature, time: new Date().getTime()};
				state.sensor2.temperatureCollection = [...state.sensor2.temperatureCollection, temperatureData];
			}
			if("humidity" in action.payload.sensor2) {
				state.sensor2.humidity = humidity;
				const humidityData = {humidity: humidity, time: new Date().getTime()};
				state.sensor2.humidityCollection = [...state.sensor2.humidityCollection, humidityData];
			}
		},
		clearSensor1Collections(state, action) {
			state.sensor1.temperatureCollection = [];
			state.sensor1.humidityCollection = [];
		},
		clearSensor2Collections(state, action) {
			state.sensor2.temperatureCollection = [];
			state.sensor2.humidityCollection = [];
		},
	},
});

export const {
	setSensor1,
	setSensor2,
	clearSensor1Collections,
	clearSensor2Collections,
	clearSensor2TemperatureCollection,
	clearSensor2HumidityCollection
} = sensorsSlice.actions;

export default sensorsSlice.reducer;
