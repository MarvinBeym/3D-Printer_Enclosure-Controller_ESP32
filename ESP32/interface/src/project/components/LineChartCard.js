import Card from "./Card";
import React from "react";
import QuickLineChart from "./QuickLineChart";


const LineChartCard = ({cardClassName, header, data, dataKey, yAxisLabel, xAxisLabel, yAxisValueSuffix}) => {

	return (
		<Card header={header} cardClassName={cardClassName}>
			<QuickLineChart
				yAxisValueSuffix={yAxisValueSuffix}
				yAxisLabel={yAxisLabel}
				xAxisLabel={xAxisLabel}
				data={data}
				dataKey={dataKey}
			/>
		</Card>
	)
}

export default LineChartCard;