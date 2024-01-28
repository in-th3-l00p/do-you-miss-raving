import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {Modal} from "@/components/Modal";
import {API} from "@/lib/constants";
import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";

interface UploadModalProps {
    opened: boolean;
    setOpened: React.Dispatch<React.SetStateAction<boolean>>;
}

export function UploadModal({opened, setOpened}: UploadModalProps) {
    const {map, setMap} = React.useContext(EditorContext);

    return (
        <Modal
            title={"Upload"}
            opened={opened}
            setOpened={setOpened}
        >
            <form onSubmit={(e) => {
                e.preventDefault();
                const form = e.target as HTMLFormElement;
                const formData = new FormData(form);
                fetch(`${API}/api/maps/${map!._id}/textures`, {
                    method: "POST",
                    body: formData,
                })
                    .then((resp) => {
                        if (resp.status === 413) {
                            alert("File too large.");
                            return;
                        }
                        return resp.json();
                    })
                    .then((texture) => {
                        const newMap = {...map!};
                        newMap.textures?.push(texture);
                        setMap(newMap);
                    })
                    .finally(() => setOpened(false));

            }}>
                <FormGroup className={"mb-4"}>
                    <label htmlFor={"label"}>Label</label>
                    <FormInput type={"text"} name={"label"} id={"label"}/>
                </FormGroup>
                <FormGroup className={"mb-8"}>
                    <label htmlFor={"file"}>File</label>
                    <FormInput type={"file"} name={"image"} id={"image"}/>
                </FormGroup>

                <button type={"submit"} className={"btn block mx-auto"}>
                    Upload
                </button>
            </form>
        </Modal>
    )
}