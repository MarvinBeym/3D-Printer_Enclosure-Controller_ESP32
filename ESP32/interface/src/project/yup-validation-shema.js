import { setIn } from 'final-form';
import { useMemo } from 'react';

/**
 * Sets the `innerError.message` in an `errors` object at the key
 * defined by `innerError.path`.
 * @param {Object} errors The object to set the error in.
 * @param {{ path: string, message: string }} innerError A `yup` field error.
 * @returns {Object} The result of setting the new error message onto `errors`.
 */
const setInError = (errors, innerError) => {
	return setIn(errors, innerError.path, innerError.message);
};

/**
 * Empty object map with no prototype. Used as default
 * value for reducing the `err.inner` array of errors
 * from a `yup~ValidationError`.
 * @type {Object}
 */
const emptyObj = Object.create(null);

/**
 * Takes a `yup` validation schema and returns a function that expects
 * a map of values to validate. If the validation passes, the function resolves to `undefined`
 * (signalling that the values are valid). If the validation doesn't pass, it resolves
 * to a map of invalid field names to errors.
 * @param {import('yup').ObjectSchema} schema `yup` schema definition.
 * @returns {(values: Object) => Promise<?Object>} An async function that expects some `values`
 *  and resolves to either `undefined` or a map of field names to error messages.
 */
export const makeValidate = schema => {
	return async function validate(values) {
		try {
			await schema.validate(values, { abortEarly: false });
		} catch (err) {
			return err.inner.reduce(setInError, emptyObj);
		}
	};
};

function useValidationSchema(schema) {
	return useMemo(() => makeValidate(schema), [schema]);
}

export default useValidationSchema;