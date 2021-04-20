import React from "react";
import moment from 'moment'
import {Paper} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import {CartesianGrid, Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis} from "recharts";

const useStyles = makeStyles((theme) => ({
	tooltip: {
		padding: "0.5rem 1rem",
	},
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
				transform="rotate(0)"
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

const QuickLineChart = ({title, data, yAxisLabel, xAxisLabel, dataKey, yAxisValueSuffix = ""}) => {
	return (
		<ResponsiveContainer height={400}>
			<LineChart data={data}>
				<CartesianGrid strokeDasharray="3 3"/>
				<YAxis
					scale="linear"
					domain={[(dataMin) => (Math.floor(dataMin) - 20), (dataMax) => (Math.ceil(dataMax) + 20)]}
					label={{value: yAxisLabel, position: "inside", dx: -25, fontSize: 20, angle: -90}}/>
				<XAxis
					tick={<CustomizedAxisTick/>}
					domain={[0, "auto"]}
					dataKey="time"
					label={{
						value: xAxisLabel,
						position: "insideRight",
						dy: 0,
						fontSize: 20
					}}
					interval={60}
					tickFormatter={(unixTime) => moment(unixTime).format('HH:mm')}
					scale="auto"
				/>
				<Tooltip content={<CustomTooltip yAxisValueSuffix={yAxisValueSuffix}/>}/>
				<Line type="monotone" dataKey={dataKey} stroke="#ff6300"/>
			</LineChart>
		</ResponsiveContainer>
	)
}

export default QuickLineChart;

/*

 */
