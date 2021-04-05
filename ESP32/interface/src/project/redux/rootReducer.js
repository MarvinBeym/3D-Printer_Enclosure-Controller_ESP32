import {combineReducers} from 'redux';
import sensorsSlice from './reducers/sensorsSlice'

export default combineReducers({
	sensors: sensorsSlice,
});