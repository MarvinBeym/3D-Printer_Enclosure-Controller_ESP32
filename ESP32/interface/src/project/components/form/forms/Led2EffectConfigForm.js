import * as yup from 'yup';
import {Form} from 'react-final-form'
import useValidationSchema from "../../../yup-validation-shema";
import SelectField from "../fields/SelectField";
import {Button} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import SwitchField from "../fields/SwitchField";
import {useDispatch} from "react-redux";
import {useEffect, useState} from "react";
import NumberField from "../fields/NumberField";
import {wsSend} from "../../../redux/reducers/webSocketSlice";


const useStyles = makeStyles((theme) => ({
	fields: {
		display: "grid",
		gridTemplateColumns: "auto auto",
		gridGap: "1rem",
		paddingBottom: "1rem",
	},
	field: {

	}
}));

const validationSchema = yup.object().shape({});

const Led2EffectConfigForm = ({effect}) => {
	const styles = useStyles();
	const dispatch = useDispatch();
	const [initialValues, setInitialValues] = useState([]);

	const name = effect.name;
	const config = effect.config;

	async function onSubmit(values) {
		let tmpValues = values;
		tmpValues["numbers"] = tmpValues.numbers.map((numberField) => {
			let key = Object.keys(numberField)[0];
			let value = parseInt(numberField[key]);

			let obj = {};
			obj[key] = value;
			return obj;
		});

		let msg = {component: "led2", command: "setEffectConfig", effect: name, value: tmpValues};
		dispatch(wsSend(msg));
	}

	useEffect(() => {
		let obj = {};
		Object.keys(config).forEach((fieldKey) => {
			obj[fieldKey] = [];
			config[fieldKey].forEach((field) => {
				let fieldObj = {};
				fieldObj[field.name] = field.value;
				obj[fieldKey] = [...obj[fieldKey], fieldObj];
			})
		})

		setInitialValues(obj);
	}, [config]);

	const validate = useValidationSchema(validationSchema);

	return (
		<Form
			initialValues={initialValues}
			onSubmit={onSubmit}
			validate={validate}
			render={({handleSubmit}) => (
				<form onSubmit={handleSubmit}>
					<div className={styles.fields}>
						{config.selects.map((selectData, index) => {
							return (
								<SelectField
									defaultValue={selectData.value}
									fieldClassName={styles.field}
									key={selectData.name}
									name={`selects[${index}]` + selectData.name}
									label={selectData.label}
									options={selectData.options}
								/>
							)
						})}
						{config.switches.map((switchData, index) => {
							return (
								<SwitchField
									defaultValue={switchData.value}
									fieldClassName={styles.field}
									key={switchData.name}
									name={`switches[${index}]` + switchData.name}
									label={switchData.label}
								/>
							)
						})}
						{config.numbers.map((numberField, index) => (
							<NumberField
								defaultValue={numberField.value}
								fieldClassName={styles.field}
								key={numberField.name}
								name={`numbers[${index}]` + numberField.name}
								label={numberField.label}
								min={numberField.min}
								max={numberField.max}
							/>
						))}
					</div>
					<Button color="primary" variant="contained" type="submit">Save</Button>
				</form>
			)}
		/>
	)
}
export default Led2EffectConfigForm;