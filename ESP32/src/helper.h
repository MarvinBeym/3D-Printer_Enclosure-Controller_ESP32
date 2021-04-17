String valueToPercentString(int value)
{
	return String(value) + "%";
}

String valueToPercentString(float value)
{
	return String(value) + "%";
}

String valueToRpmString(int value)
{
	return String(value) + "rpm";
}

String valueToTempString(int value)
{
	return String(value) + (char) 176 + "C";
}

String valueToTempString(float value)
{
	return String(value) + (char) 176 + "C";
}

int newMap(int value, int inMin, int inMax, int outMin, int outMax) {
	int mappedValue = (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	if(mappedValue <= outMin) {
		mappedValue = outMin;
	}
	if(mappedValue >= outMax) {
		mappedValue = outMax;
	}
	return mappedValue;
}