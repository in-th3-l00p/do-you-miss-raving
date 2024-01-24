"use client"

import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";

export default function Login() {
    return (
        <form
            onSubmit={(e) => {
                e.preventDefault();
            }}
            className={"background flex-grow flex flex-col items-center justify-center gap-8 pt-8 px-4"}
        >
            <h1 className={"text-2xl md:text-4xl"}>Login</h1>

            <FormGroup>
                <label htmlFor={"email"}>Email</label>
                <FormInput type={"email"} id={"email"} name={"email"} />
            </FormGroup>

            <FormGroup>
                <label htmlFor={"password"}>Password</label>
                <FormInput type={"password"} id={"password"} name={"password"} />
            </FormGroup>

            <button type={"submit"} className={"btn"}>Login</button>
        </form>
    );
}