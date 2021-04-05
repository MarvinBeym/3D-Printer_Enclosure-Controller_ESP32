import React from "react";
import {CartesianGrid, Legend, Line, LineChart, Tooltip, XAxis, YAxis} from "recharts";
import moment from 'moment'
import {Paper} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";

const useStyles = makeStyles((theme) => ({
	tooltip: {
		padding: "0.5rem 1rem",
	}
}));

const CustomizedAxisTick = ({x, y, payload}) => {
	return (
		<g transform={`translate(${x},${y})`}>
			<text
				x={0}
				y={0}
				dy={16}
				textAnchor="end"
				fill="#666"
				transform="rotate(-35)"
			>
				{moment(payload.value).format('HH:mm:ss')}
			</text>
		</g>
	);
};

//@ts-ignore
const CustomTooltip = ({active, payload, label, yAxisValueSuffix}) => {
	const styles = useStyles();
	if (active && payload && payload.length) {
		return (
			<Paper elevation={5} className={styles.tooltip}>
				<p>{moment(label).format('HH:mm:ss')}</p>
				<p>{payload[0].value + yAxisValueSuffix}</p>
			</Paper>
		);
	}

	return null;
};

const QuickLineChart = ({data, yAxisLabel, xAxisLabel, dataKey, yAxisValueSuffix = ""}) => {

	return (
		<LineChart
			margin={{bottom: 20, left: 20}}
			width={400}
			height={400}
			data={data}>
			<YAxis
				domain={[(dataMin) => (Math.floor(dataMin) - 20), (dataMax) => (Math.ceil(dataMax) + 20)]}
				label={{value: yAxisLabel, position: "inside", dx: -15, fontSize: 20, angle: -90}}/>
			<XAxis
				tick={<CustomizedAxisTick/>}
				domain={[0, "auto"]}
				dataKey="time"
				label={{
					value: xAxisLabel,
					position: "insideBottomRight",
					dy: 15,
					fontSize: 20
				}}
				interval={60}
				tickFormatter={(unixTime) => moment(unixTime).format('HH:mm')}
				scale="time"
			/>
			<Legend/>
			<Tooltip content={<CustomTooltip yAxisValueSuffix={yAxisValueSuffix}/>}/>
			<CartesianGrid strokeDasharray="3 3" stroke="#5c5c5c"/>
			<Line type="monotone" dataKey={dataKey} stroke="#ff6300"/>
		</LineChart>
	)
}

export default QuickLineChart;