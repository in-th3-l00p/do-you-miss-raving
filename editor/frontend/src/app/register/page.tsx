"use client"

import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";

export default function Register() {
    return (
        <form
            onSubmit={(e) => {
                e.preventDefault();
            }}
            className={"background flex-grow flex flex-col items-center justify-center gap-8 pt-8 px-4"}
        >
            <h1 className={"text-2xl md:text-4xl"}>Register</h1>

            <FormGroup>
                <label htmlFor={"email"}>Email</label>
                <FormInput type={"email"} id={"email"} name={"email"} />
            </FormGroup>

            <FormGroup>
                <label htmlFor={"name"}>Name</label>
                <FormInput type={"text"} id={"name"} name={"name"} />
            </FormGroup>

            <FormGroup>
                <label htmlFor={"password"}>Password</label>
                <FormInput type={"password"} id={"password"} name={"password"} />
            </FormGroup>

            <FormGroup>
                <label htmlFor={"password_confirmed"}>Confirm password</label>
                <FormInput type={"password"} id={"password_confirmed"} name={"password_confirmed"} />
            </FormGroup>

            <button type={"submit"} className={"btn"}>Register</button>
        </form>
    );
}