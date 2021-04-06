import {combineReducers} from 'redux';
import sensorsSlice from './reducers/sensorsSlice'
import ledsSlice from "./reducers/ledsSlice";

export default combineReducers({
	sensors: sensorsSlice,
	leds: ledsSlice,
});