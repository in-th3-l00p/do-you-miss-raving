"use client";

import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";

export default function Register() {
    return (
        <form
            className={"background flex-grow flex justify-center items-center flex-col gap-8 px-4"}
            onSubmit={(e) => {
                e.preventDefault();
            }}
        >
            <h1 className={"text-4xl"}>Register</h1>

            <FormGroup>
                <label className={"text-2xl mb-4"} htmlFor="username">Username:</label>
                <FormInput
                    id={"username"} name={"username"} type={"text"}
                />
            </FormGroup>

            <FormGroup>
                <label className={"text-2xl mb-4"} htmlFor="email">Email:</label>
                <FormInput
                    id={"email"} name={"email"} type={"email"}
                />
            </FormGroup>

            <FormGroup>
                <label className={"text-2xl mb-4"} htmlFor="password">Password:</label>
                <FormInput
                    id={"password"} name={"password"} type={"password"}
                />
            </FormGroup>

            <FormGroup>
                <label className={"text-2xl mb-4"} htmlFor="password_confirmed">
                    Confirm password:
                </label>
                <FormInput
                    id={"password_confirmed"}
                    name={"password_confirmed"}
                    type={"password"}
                />
            </FormGroup>

            <button type={"submit"} className={"btn"}>Register</button>
        </form>
    )
}
