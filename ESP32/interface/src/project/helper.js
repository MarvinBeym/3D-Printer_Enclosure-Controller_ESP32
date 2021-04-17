import {useEffect, useRef} from 'react'
import {authorizedFetch} from "../authentication";
import {ENDPOINT_ROOT} from "../api";

export const fetchBackend = (fetchEndpoint) => {
	return authorizedFetch(ENDPOINT_ROOT + fetchEndpoint).then((response) => {
		return response.json();
	})
}

export const map = (value, inMin, inMax, outMin, outMax) => {
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

export const useInterval = (callback, delay) => {
	const savedCallback = useRef(callback)

	// Remember the latest callback if it changes.
	useEffect(() => {
		savedCallback.current = callback
	}, [callback])

	// Set up the interval.
	useEffect(() => {
		// Don't schedule if no delay is specified.
		if (delay === null) {
			return
		}

		const id = setInterval(() => savedCallback.current(), delay)

		return () => clearInterval(id)
	}, [delay])
}

export const setLoadingState = (state, action) => {
	state.status = 'loading';
}
export const setFailureState = (state, action) => {
	state.status = 'failed'
	state.error = action.error.message
}