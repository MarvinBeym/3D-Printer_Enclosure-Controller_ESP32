import {createSlice} from "@reduxjs/toolkit";

const initialState = {
	fan1: {
		percent: 0,
		rpm: 0,
		dutyCycle: 0,
	},
	fan2: {
		percent: 0,
		rpm: 0,
		dutyCycle: 0,
	},
	status: 'idle',
}

export const selectFan1 = (state) => state.fans.fan1;
export const selectFan2 = (state) => state.fans.fan2;
export const selectFans = (state) => [state.fans.fan1, state.fans.fan2];
const fansSlice = createSlice({
	name: 'fans',
	initialState,
	reducers: {
		setFan1(state, action) {
			let percent = action.payload.fan1?.percent;
			let rpm = action.payload.fan1?.rpm;
			let dutyCycle = action.payload.fan1?.dutyCycle;
			if(percent) {
				state.fan1.percent = percent;
			}
			if(rpm) {
				state.fan1.rpm = rpm;
			}
			if(dutyCycle) {
				state.fan1.dutyCycle = dutyCycle;
			}
		},
		setFan2(state, action) {
			let percent = action.payload.fan2?.percent;
			let rpm = action.payload.fan2?.rpm;
			let dutyCycle = action.payload.fan2?.dutyCycle;
			if(percent) {
				state.fan2.percent = percent;
			}
			if(rpm) {
				state.fan2.rpm = rpm;
			}
			if(dutyCycle) {
				state.fan2.dutyCycle = dutyCycle;
			}
		},
	},
});

export const {setFan1, setFan2} = fansSlice.actions;

export default fansSlice.reducer;
