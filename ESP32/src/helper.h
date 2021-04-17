#pragma once

class Helper {
	public:
		static String valueToPercentString(int value)
		{
			return String(value) + "%";
		}

		static String valueToPercentString(float value)
		{
			return String(value) + "%";
		}

		static String valueToRpmString(int value)
		{
			return String(value) + "rpm";
		}

		static String valueToTempString(int value)
		{
			return String(value) + (char) 176 + "C";
		}

		static String valueToTempString(float value)
		{
			return String(value) + (char) 176 + "C";
		}

		static int newMap(int value, int inMin, int inMax, int outMin, int outMax) {
			int mappedValue = (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
			if(mappedValue <= outMin) {
				mappedValue = outMin;
			}
			if(mappedValue >= outMax) {
				mappedValue = outMax;
			}
			return mappedValue;
		}
};