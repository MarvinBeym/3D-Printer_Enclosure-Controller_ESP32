import {createAsyncThunk, createSlice} from "@reduxjs/toolkit";
import {fetchBackend, setFailureState, setLoadingState} from "../../helper";

const initialState = {
	led1: {
		currentState: false
	},
	led2: {
		currentEffect: 0,
		effects: [
			{name: "none", id: 0}
		]
	},
	status: 'idle',
}

export const selectLed1 = (state) => state.leds.led1;
export const selectLed2 = (state) => state.leds.led2;

export const fetchLed1State = createAsyncThunk('led1/fetchCurrentState', async () => {
	return fetchBackend("leds/led1/currentState")
		.then((data) => {
			return data;
		})
		.catch((error) => {
			return Promise.reject(error);
		});
});

export const changeLed1State = createAsyncThunk('led1/changeState', async (state) => {
	return fetchBackend(`leds/led1/changeState/${state ? 1 : 0}`)
		.then((data) => {
			return data;
		})
		.catch((error) => {
			return Promise.reject(error);
		});
});

export const changeLed2Effect = createAsyncThunk('led2/changeEffect', async (effectId) => {
	return fetchBackend(`leds/led2/changeEffect/${effectId}`)
		.then((data) => {
			return data;
		})
		.catch((error) => {
			return Promise.reject(error);
		});
});

export const fetchCurrentLed2Effect = createAsyncThunk('led2/fetchCurrentEffect', async () => {
	return fetchBackend("leds/led2/currentEffect")
		.then((data) => {
			return data;
		})
		.catch((error) => {
			return Promise.reject(error);
		});
});


export const fetchLed2Effects = createAsyncThunk('led2/fetchEffects', async () => {
	return fetchBackend("leds/led2/effects")
		.then((data) => {
			return data;
		})
		.catch((error) => {
			return Promise.reject(error);
		});
});

const ledsSlice = createSlice({
	name: 'leds',
	initialState,
	reducers: {},
	extraReducers: {
		[fetchLed2Effects.pending]: setLoadingState,
		[fetchCurrentLed2Effect.pending]: setLoadingState,
		[changeLed2Effect.pending]: setLoadingState,
		[changeLed1State.pending]: setLoadingState,
		[fetchLed1State.pending]: setLoadingState,

		[fetchLed2Effects.rejected]: setFailureState,
		[fetchCurrentLed2Effect.rejected]: setFailureState,
		[changeLed2Effect.rejected]: setFailureState,
		[changeLed1State.rejected]: setFailureState,
		[fetchLed1State.rejected]: setLoadingState,

		[fetchLed2Effects.fulfilled]: (state, action) => {
			state.status = "succeeded";
			state.led2.effects = action.payload.led2.effects;
		},
		[fetchCurrentLed2Effect.fulfilled]: (state, action) => {
			state.status = "succeeded";
			state.led2.currentEffect = action.payload.led2.currentEffect;
		},
		[changeLed2Effect.fulfilled]: (state, action) => {
			state.status = "succeeded";
			state.led2.currentEffect = action.payload.led2.currentEffect
		},
		[changeLed1State.fulfilled]: (state, action) => {
			state.status = "succeeded";
			state.led1.currentState = action.payload.led1.currentState;
		},
		[fetchLed1State.fulfilled]: (state, action) => {
			state.status = "succeeded";
			state.led1.currentState = action.payload.led1.currentState;
		}
	}
});

export default ledsSlice.reducer;
