import {useState} from "react";
import {fetchBackend, useInterval} from "../helper";


const IntervalFetchWrapper = ({fetchEndpoint, render, fetchInterval = 2000}) => {
	const [fetchData, setFetchData] = useState(null);

	useInterval(() => {
		fetchBackend(fetchEndpoint).then((data) => {
			setFetchData(data);
		})
	}, fetchInterval);

	return render(fetchData);
}

export default IntervalFetchWrapper;