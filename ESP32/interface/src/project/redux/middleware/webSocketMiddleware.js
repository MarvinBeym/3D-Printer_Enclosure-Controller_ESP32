/* eslint-disable default-case */
import {setLed1, setLed2} from "../reducers/ledsSlice";
import {setSensor1, setSensor2} from "../reducers/sensorsSlice";
import {setFan1, setFan2} from "../reducers/fansSlice";

const webSocketMiddleware = (store) => (next) => (action) => {
	if (action.type !== "ENCLOSURE_CONTROLLER::MESSAGE") {
		next(action);
		return;
	}

	let json;
	try {
		json = JSON.parse(action.payload.message)
	} catch {
		next(action);
		return;
	}

	const dispatch = store.dispatch;

	Object.keys(json).forEach((jsonKey) => {
		switch (jsonKey) {
			case "led1":
				dispatch(setLed1(json));
				break;
			case "led2":
				dispatch(setLed2(json));
				break;
			case "sensor1":
				dispatch(setSensor1(json));
				break;
			case "sensor2":
				dispatch(setSensor2(json));
				break;
			case "fan1":
				dispatch(setFan1(json));
				break;
			case "fan2":
				dispatch(setFan2(json));
				break;
		}
	})

	next(action);
}
export default webSocketMiddleware;