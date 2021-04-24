/* eslint-disable default-case */
import {setLed1, setLed2} from "../reducers/ledsSlice";
import {setSensor1, setSensor2} from "../reducers/sensorsSlice";
import {setFan1, setFan2} from "../reducers/fansSlice";
import {removeExecutedCall} from "../reducers/webSocketSlice";
import {setConfiguration} from "../reducers/configurationSlice";

const webSocketMiddleware = (store) => (next) => (action) => {
	const dispatch = store.dispatch;
	switch (action.type) {
		case "ENCLOSURE_CONTROLLER::MESSAGE":
			try {
				let json = JSON.parse(action.payload.message);
				checkCommandExecutedSuccessfully(dispatch, json);
				handleWebSocketMessages(dispatch, json);
			} catch {
				next(action);
				return;
			}
			break;
		default:
			break;
	}
	next(action);
}
export default webSocketMiddleware;

/**
 * Checks if the command was executed successfully
 * and removes the command from the executedCalls array allowing the command to be executed again
 * @param dispatch
 * @param json
 */
const checkCommandExecutedSuccessfully = (dispatch, json) => {
	if (json?.message === "Executed command" && json?.status === "success") {
		dispatch(removeExecutedCall({command: json.command, component: json.component}));
	}
}

const handleWebSocketMessages = (dispatch, json) => {
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
			case "configuration":
				dispatch(setConfiguration(json));
		}
	})

}