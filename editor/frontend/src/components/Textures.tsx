import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {API} from "@/lib/constants";
import * as Icon from "react-feather";
import {Panel} from "@/components/Editor";

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
            onClick={() => {
                setOpened(false);
            }}
        >
            <div className={"bg-federal-blue rounded-md shadow-md p-8 w-full max-w-[600px]"}>
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

export default function Textures() {
    const {map, setMap} = React.useContext(EditorContext);
    const [modalOpened, setModalOpened] = React.useState(true);

    if (!map)
        return (
            <Panel title={"Textures"}/>
        );
    return (
        <>
            <Modal title={"Upload"} opened={modalOpened} setOpened={setModalOpened}>
            </Modal>
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
                            "flex justify-center items-center bg-oxford-blue w-48 rounded-md shadow-md " +
                            "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                        }
                    >
                        <Icon.Plus size={"80px"}/>
                    </button>
                </div>
            </Panel>
        </>
    );
}