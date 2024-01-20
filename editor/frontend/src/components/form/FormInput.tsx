import React, {HTMLInputTypeAttribute} from "react";

interface FormInputProps {
    type: HTMLInputTypeAttribute;
    name?: string;
    id?: string;
}

export default function FormInput({ type, name, id }: FormInputProps) {
    return (
        <input
            type={type}
            name={name}
            id={id}
            className={"input"}
        />
    );
}