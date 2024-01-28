import * as Icon from "react-feather";
import React from "react";

interface ModalProps {
    title: string;
    children?: React.ReactNode;
    opened: boolean;
    setOpened: React.Dispatch<React.SetStateAction<boolean>>
}

export function Modal({title, children, opened, setOpened}: ModalProps) {
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
                        <Icon.X size={"16px"}/>
                    </button>
                </div>
                {children}
            </div>
        </section>
    );
}