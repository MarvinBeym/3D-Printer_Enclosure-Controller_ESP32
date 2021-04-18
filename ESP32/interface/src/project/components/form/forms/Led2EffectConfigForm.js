import * as yup from 'yup';
import {Form} from 'react-final-form'
import useValidationSchema from "../../../yup-validation-shema";
import SelectField from "../fields/SelectField";
import {Button} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import SwitchField from "../fields/SwitchField";
import {useDispatch} from "react-redux";
import {wsSend} from "../../../redux/reducers/webSocketSlice";


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
	async function onSubmit(values) {
		let msg = {component: "led2", command: "setEffectConfig", effect: effectName, value: values};
		dispatch(wsSend(msg));
	}

	const validate = useValidationSchema(validationSchema);

	return (
		<Form
			onSubmit={onSubmit}
			validate={validate}
			render={({handleSubmit}) => (
				<form onSubmit={handleSubmit}>
					<div className={styles.fields}>
						{selects.map((selectData) => {
							return (
								<SelectField
									fieldClassName={styles.field}
									key={selectData.name}
									name={selectData.name}
									label={selectData.label}
									defaultValue={selectData.value}
									options={selectData.options}
								/>
							)
						})}
						{switches.map((switchData) => {
							return (
								<SwitchField fieldClassName={styles.field} key={switchData.name} name={switchData.name}
											 label={switchData.label}/>
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