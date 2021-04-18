import * as yup from 'yup';
import {Form} from 'react-final-form'
import useValidationSchema from "../../../yup-validation-shema";
import SelectField from "../fields/SelectField";
import {Button} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import SwitchField from "../fields/SwitchField";
import {useDispatch} from "react-redux";
import {wsSend} from "../../../redux/reducers/webSocketSlice";
import {useEffect, useState} from "react";


const useStyles = makeStyles((theme) => ({
	fields: {
		paddingBottom: "2rem",
	},
	field: {
		margin: "0.5rem 0",
	}
}));

const validationSchema = yup.object().shape({
	//username: yup.string().required("validation.missing.username"),
	//password: yup.string().min(6, "validation.lengthNotMet.password").required("validation.missing.password")
});

const Led2EffectConfigForm = ({effectName, switches, selects}) => {
	const styles = useStyles();
	const dispatch = useDispatch();
	const [initialValues, setInitialValues] = useState([]);

	async function onSubmit(values) {
		let msg = {component: "led2", command: "setEffectConfig", effect: effectName, value: values};
		dispatch(wsSend(msg));
	}

	useEffect(() => {
		let selectInitialValues = selects.map((selectData) => {
			let obj = {};
			obj[selectData.name] = selectData.value
			return obj;
		})

		let switchInitialValues = switches.map((switchData) => {
			let obj = {};
			obj[switchData.name] = switchData.state;
			return obj;
		})

		setInitialValues({selects: selectInitialValues, switches: switchInitialValues})
	}, [selects, switches]);

	const validate = useValidationSchema(validationSchema);

	return (
		<Form
			initialValues={initialValues}
			onSubmit={onSubmit}
			validate={validate}
			render={({handleSubmit}) => (
				<form onSubmit={handleSubmit}>
					<div className={styles.fields}>
						{selects.map((selectData, index) => {
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
						{switches.map((switchData, index) => {
							return (
								<SwitchField
									defaultValue={switchData.state}
									fieldClassName={styles.field}
									key={switchData.name}
									name={`switches[${index}]` + switchData.name}
									label={switchData.label}
								/>
							)
						})}
					</div>
					<Button color="primary" variant="contained" type="submit">Save</Button>
				</form>
			)}
		/>
	)
}
export default Led2EffectConfigForm;