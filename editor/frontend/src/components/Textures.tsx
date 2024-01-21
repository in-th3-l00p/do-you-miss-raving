import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {API} from "@/lib/constants";
import * as Icon from "react-feather";
import FormGroup from "@/components/form/FormGroup";
import FormInput from "@/components/form/FormInput";
import {Panel} from "@/components/Panel";

interface ModalProps {
    title: string;
    children?: React.ReactNode;
    opened: boolean;
    setOpened: React.Dispatch<React.SetStateAction<boolean>>
}

function Modal({ title, children, opened, setOpened }: ModalProps) {
    if (!opened)
        return <></>

    return (
        <section
            className={"fixed z-40 top-0 left-0 w-screen h-screen flex items-center justify-center"}
            style={{background: "rgba(0, 0, 0, 0.5)"}}
        >
            <div className={"bg-federal-blue rounded-md shadow-md p-8 w-full max-w-[600px] z-50 z-50 z-50 z-50"}>
                <div className={"flex items-center justify-between mb-8"}>
                    <h2 className={"text-2xl"}>{title}</h2>
                    <button
                        type={"button"}
                        className={"btn"}
                        onClick={() => {
                            setOpened(false);
                        }}
                    >
                        <Icon.X size={"16px"} />
                    </button>
                </div>
                {children}
            </div>
        </section>
    );
}

interface UploadModalProps {
    opened: boolean;
    setOpened: React.Dispatch<React.SetStateAction<boolean>>;
}

function UploadModal({ opened, setOpened }: UploadModalProps) {
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
                    <FormInput type={"text"} name={"label"} id={"label"} />
                </FormGroup>
                <FormGroup className={"mb-8"}>
                    <label htmlFor={"file"}>File</label>
                    <FormInput type={"file"} name={"image"} id={"image"} />
                </FormGroup>

                <button type={"submit"} className={"btn block mx-auto"}>
                    Upload
                </button>
            </form>
        </Modal>
    )
}

export default function Textures() {
    const {map, setMap} = React.useContext(EditorContext);
    const [uploadOpened, setUploadOpened] = React.useState(false);

    if (!map)
        return (
            <Panel title={"Textures"}/>
        );
    return (
        <>
            <UploadModal
                opened={uploadOpened}
                setOpened={setUploadOpened}
            />
            <Panel title={"Textures"}>
                <div className={"flex flex-wrap gap-8"}>
                    {map.textures?.map((texture, index) => (
                        <button
                            type={"button"}
                            key={index}
                            className={
                                "text-center p-4 bg-oxford-blue rounded-md w-48 " +
                                "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                            }
                        >
                            <img
                                src={`${API}/api/maps/${texture.path}`}
                                alt={texture.label}
                                className={"aspect-square object-contain w-32 mx-auto my-auto"}
                            />
                            <p className={"mt-4"}>{texture.label}</p>
                        </button>
                    ))}

                    <button
                        type={"button"}
                        className={
                            "flex justify-center items-center bg-oxford-blue aspect-square w-48 rounded-md shadow-md " +
                            "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                        }
                        onClick={() => {
                            setUploadOpened(true);
                        }}
                    >
                        <Icon.Plus size={"80px"}/>
                    </button>
                </div>
            </Panel>
        </>
    );
}