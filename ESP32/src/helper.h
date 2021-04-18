/*
  Class with static helper functions
  Created by Marvin Beym, 17.04.2021.
*/
#pragma once

class Helper {
	public:
		static boolean isNumeric(String str) {
			unsigned int stringLength = str.length();

			if (stringLength == 0) {
				return false;
			}

			boolean seenDecimal = false;

			for(unsigned int i = 0; i < stringLength; ++i) {
				if (isDigit(str.charAt(i))) {
					continue;
				}

				if (str.charAt(i) == '.') {
					if (seenDecimal) {
						return false;
					}
					seenDecimal = true;
					continue;
				}
				return false;
			}
			return true;
		}
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