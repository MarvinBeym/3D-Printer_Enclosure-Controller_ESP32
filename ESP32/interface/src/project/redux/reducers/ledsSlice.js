import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	led1State: false,
	led2: {},
	status: 'idle',
}

export const selectLed1State = (state) => state.leds.led1State;
export const selectLed2 = (state) => state.leds.led2;
const ledsSlice = createSlice({
	name: 'leds',
	initialState,
	reducers: {
		setLed1State(state, action) {
			state.led1State = action.payload;
		},
		setLed2(state, action) {
			state.led2 = action.payload;
		},
	},
});

export const {setLed1State, setLed2} = ledsSlice.actions;

export default ledsSlice.reducer;
