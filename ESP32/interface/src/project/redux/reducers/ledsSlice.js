import {createSlice} from "@reduxjs/toolkit";

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
		setLed1(state, action) {
			if ("state" in action.payload.led1) {
				state.led1.state = action.payload.led1.state;
			}
		},
		setLed2(state, action) {
			if ("currentEffect" in action.payload.led2) {
				state.led2.currentEffect = action.payload.led2.currentEffect;
			}
			if ("effects" in action.payload.led2) {

				state.led2.effects = action.payload.led2.effects;
			}
		},
	},
});

export const {setLed1, setLed2} = ledsSlice.actions;

export default ledsSlice.reducer;
