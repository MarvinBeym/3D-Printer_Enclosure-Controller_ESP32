import {combineReducers} from 'redux';
import sensorsSlice from './reducers/sensorsSlice'
import ledsSlice from "./reducers/ledsSlice";
import fansSlice from "./reducers/fansSlice";

export default combineReducers({
	sensors: sensorsSlice,
	leds: ledsSlice,
	fans: fansSlice,
});