import {combineReducers} from 'redux';
import sensorsSlice from './reducers/sensorsSlice'
import ledsSlice from "./reducers/ledsSlice";
import fansSlice from "./reducers/fansSlice";
import webSocketSlice from "./reducers/webSocketSlice";

export default combineReducers({
	webSocket: webSocketSlice,
	sensors: sensorsSlice,
	leds: ledsSlice,
	fans: fansSlice,
});