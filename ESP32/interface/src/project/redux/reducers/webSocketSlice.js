import {createAsyncThunk, createSlice} from "@reduxjs/toolkit";
import {send} from "@giantmachines/redux-websocket";

const initialState = {
	executedCalls: [],
	status: 'idle',
}

export const selectSensor1 = (state) => state.sensors.sensor1;
export const selectSensor2 = (state) => state.sensors.sensor2;

export const wsSend = createAsyncThunk('webSocket/wsSend', async (data, {dispatch, getState}) => {
	let newCommand = data.command;
	let newComponent = data.component;
	let state = getState().webSocket;

	let alreadyExecuted = false;
	state.executedCalls.forEach((call) => {
		let command = call.command;
		let component = call.component;
		if (command === newCommand && component === newComponent) {
			alreadyExecuted = true;
		}
	})

	return new Promise((resolve, reject) => {
		if (!alreadyExecuted) {
			dispatch(send(data, process.env.REACT_APP_WEB_SOCKET_PREFIX));
			resolve(data);
		} else {
			reject(data);
		}
	})
});
const webSocketSlice = createSlice({
	name: 'webSocket',
	initialState,
	extraReducers: {
		[wsSend.pending]: (state, action) => {
			state.status = 'loading';
		},
		[wsSend.rejected]: (state, action) => {
			state.status = 'failed';
			//Already executed
		},
		[wsSend.fulfilled]: (state, action) => {
			state.status = 'succeeded';

			//Not yet executed. Store executed call. To be cleaned by middleware
			let newCommand = action.payload.command;
			let newComponent = action.payload.component;
			state.executedCalls = [...state.executedCalls, {command: newCommand, component: newComponent}];
		}
	},
	reducers: {
		removeExecutedCall(state, action) {
			let call = action.payload;

			let newExecutedCallsArr = [];

			state.executedCalls.forEach((executedCall) => {
				if (executedCall.command === call.command && executedCall.component === call.component) {
					//Current call
				} else {
					//Not current call
					newExecutedCallsArr.push(executedCall);
				}
			})
			state.executedCalls = newExecutedCallsArr;
		}
	},
});

export const {removeExecutedCall} = webSocketSlice.actions;

export default webSocketSlice.reducer;
