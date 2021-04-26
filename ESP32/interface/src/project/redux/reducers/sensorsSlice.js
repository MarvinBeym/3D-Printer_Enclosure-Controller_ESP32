import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	sensor1: {
		temperature: 0,
		humidity: 0,
		temperatureCollection: [],
		humidityCollection: [],
		tempDanger: {
			enabled: false,
			threshold: 100,
		},
	},
	sensor2: {
		temperature: 0,
		humidity: 0,
		temperatureCollection: [],
		humidityCollection: [],
		tempDanger: {
			enabled: false,
			threshold: 100,
			exceeded: false
		},
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
			updateSensor(state, "sensor1", action.payload.sensor1);
		},
		setSensor2(state, action) {
			updateSensor(state, "sensor2", action.payload.sensor2);
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

const updateSensor = (state, sensorName, sensorData) => {
	let temperature = sensorData?.temperature;
	let humidity = sensorData?.humidity;
	let tempDanger = sensorData?.tempDanger;

	if ("temperature" in sensorData) {
		state[sensorName].temperature = temperature;
		const temperatureData = {temperature: temperature, time: new Date().getTime()};

		//Check temp danger
		if (state[sensorName].tempDanger.enabled) {
			state[sensorName].tempDanger.exceeded = temperature > state[sensorName].tempDanger.threshold;
		} else if (state[sensorName].tempDanger.exceeded) {
			state[sensorName].tempDanger.exceeded = false;
		}

		state[sensorName].temperatureCollection = [...state[sensorName].temperatureCollection, temperatureData];
	}
	if ("humidity" in sensorData) {
		state[sensorName].humidity = humidity;
		state[sensorName].humidityCollection = [...state[sensorName].humidityCollection, humidity]
		const humidityData = {humidity: humidity, time: new Date().getTime()};
		state[sensorName].humidityCollection = [...state[sensorName].humidityCollection, humidityData];
	}
	if ("tempDanger" in sensorData) {
		state[sensorName].tempDanger.enabled = tempDanger.enabled;
		state[sensorName].tempDanger.threshold = tempDanger.threshold;

		if (!tempDanger.enabled) {
			state[sensorName].tempDanger.exceeded = false;
		} else {
			state[sensorName].tempDanger.exceeded = checkTempDangerExceeded(state[sensorName]);
		}
	}
}

const checkTempDangerExceeded = (sensor) => {
	return sensor.temperature > sensor.tempDanger.threshold;
}

export const {
	setSensor1,
	setSensor2,
	clearSensor1Collections,
	clearSensor2Collections,
	clearSensor2TemperatureCollection,
	clearSensor2HumidityCollection
} = sensorsSlice.actions;

export default sensorsSlice.reducer;
