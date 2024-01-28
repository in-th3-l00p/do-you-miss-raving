"use client";

import Title from "@/components/Title";
import FormInput from "@/components/form/FormInput";
import FormGroup from "@/components/form/FormGroup";
import {useState} from "react";
import FormError from "@/components/form/FormError";
import {API} from "@/lib/constants";

export default function CreateMap() {
    const [errors, setErrors] = useState<Record<string, string>>({});

    return (
        <div className={"background flex-grow"}>
            <Title text={"Create Map"} />
            <form
                className={"flex flex-col items-center justify-center gap-8"}
                onSubmit={(e) => {
                    e.preventDefault();
                    setErrors({});
                    const errors: Record<string, string> = {};

                    const formData = new FormData(e.target as HTMLFormElement);
                    const name = formData.get("name") as string;
                    const width = parseInt(formData.get("width") as string);
                    const height = parseInt(formData.get("height") as string);

                    if (name.length < 1)
                        errors["name"] = "Name must be at least 1 character long";

                    if (width < 1 || width > 100 || isNaN(width))
                        errors["width"] = "Width must be between 1 and 100";

                    if (height < 1 || height > 100 || isNaN(height))
                        errors["height"] = "Height must be between 1 and 100";


                    if (Object.keys(errors).length !== 0) {
                        setErrors(errors);
                        return;
                    }

                    fetch(`${API}/api/maps`, {
                        method: "POST",
                        headers: {
                            "Content-Type": "application/json"
                        },
                        body: JSON.stringify({ name, width, height })
                    }).then((res) => {
                        if (res.status === 200) {
                            window.location.href = "/admin/maps";
                        } else {
                            setErrors({ name: "Name already exists" });
                        }
                    });
                }}
            >
                <FormGroup>
                    <label htmlFor="name">Name:</label>
                    <FormInput type={"text"} name={"name"} id={"name"} />
                    {errors["name"] && <FormError>{errors["name"]}</FormError>}
                </FormGroup>
                <FormGroup>
                    <label htmlFor="width">Width:</label>
                    <FormInput type={"number"} name={"width"} id={"width"} />
                    {errors["width"] && <FormError>{errors["width"]}</FormError>}
                </FormGroup>
                <FormGroup>
                    <label htmlFor="height">Height:</label>
                    <FormInput type={"number"} name={"height"} id={"height"} />
                    {errors["height"] && <FormError>{errors["height"]}</FormError>}
                </FormGroup>

                <button
                    type={"submit"}
                    className={"btn"}
                >
                    Submit
                </button>
            </form>
        </div>
    );
}