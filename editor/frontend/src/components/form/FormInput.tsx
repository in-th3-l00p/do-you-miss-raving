import React, {HTMLInputTypeAttribute} from "react";

interface FormInputProps {
    type: HTMLInputTypeAttribute;
    name?: string;
    id?: string;
    defaultValue?: string;
}

export default function FormInput({ type, name, id, defaultValue }: FormInputProps) {
    return (
        <input
            type={type}
            name={name}
            id={id}
            className={"input"}
            defaultValue={defaultValue}
        />
    );
}