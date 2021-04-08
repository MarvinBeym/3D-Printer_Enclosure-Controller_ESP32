import {createAsyncThunk, createSlice} from "@reduxjs/toolkit";
import {fetchBackend, setFailureState, setLoadingState} from "../../helper";

const initialState = {
	led1: {
		state: false
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

const ledsSlice = createSlice({
	name: 'leds',
	initialState,
	reducers: {
		setLed2CurrentEffect(state, action) {
			if(action.payload.led2.currentEffect) {
				state.led2.currentEffect = action.payload.led2.currentEffect;
			}
		},
		setLed2Effects(state, action) {
			if (action.payload.led2.effects) {
				state.led2.effects = action.payload.led2.effects;
			}
		},
		setLed1State(state, action) {
			if(action.payload.led1.state) {
				state.led1.state = action.payload.led1.state;
			}
		}
	},
});

export const {setLed2CurrentEffect, setLed2Effects, setLed1State} = ledsSlice.actions;

export default ledsSlice.reducer;
