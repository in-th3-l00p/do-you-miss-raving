import React from "react";
import {Map, Texture} from "@/lib/types";
import {Modal} from "@/components/Modal";
import {API} from "@/lib/constants";
import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";

function onUpdateFormSubmit(
    e: React.FormEvent<HTMLFormElement>,
    map: Map,
    texture: Texture,
    setMap: React.Dispatch<React.SetStateAction<Map | null>>,
    setOpened: React.Dispatch<React.SetStateAction<boolean>>
) {
    e.preventDefault();
    const form = e.target as HTMLFormElement;
    const formData = new FormData(form);
    if (formData.has("image"))
        formData.set("label", texture.label);
    fetch(`${API}/api/maps/${map._id}/textures/${texture._id}`, {
        method: "PUT",
        body: formData,
    })
        .then((resp) => {
            return resp.json();
        })
        .then((textures) =>
            setMap(map => ({ ...map!, textures }))
        )
        .finally(() => setOpened(false));
}

interface EditModelProps {
    map: Map;
    texture: Texture;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;
    open: boolean;
    setOpened: React.Dispatch<React.SetStateAction<boolean>>;
}

export default function EditModal({ map, texture, setMap, open, setOpened }: EditModelProps) {
    return (
        <Modal
            title={"Edit texture " + texture.label}
            opened={open}
            setOpened={setOpened}
        >
            <form
                onSubmit={e => onUpdateFormSubmit(e, map, texture, setMap, setOpened)}
                className={"pb-8 border-b mb-8"}
            >
                <FormGroup className={"mb-4"}>
                    <label htmlFor={"label"}>Label</label>
                    <FormInput
                        type={"text"}
                        name={"label"}
                        id={"label"}
                        defaultValue={texture.label}
                    />
                </FormGroup>

                <button type={"submit"} className={"btn block"}>
                    Submit
                </button>
            </form>

            <form onSubmit={e => onUpdateFormSubmit(e, map, texture, setMap, setOpened)}>
                <FormGroup className={"mb-8"}>
                    <label htmlFor={"file"}>File</label>
                    <FormInput type={"file"} name={"image"} id={"image"}/>
                </FormGroup>

                <button type={"submit"} className={"btn block"}>
                    Upload
                </button>
            </form>
        </Modal>
    );
}