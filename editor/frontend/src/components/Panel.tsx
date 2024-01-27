import React from "react";

export interface PanelProps {
    title: string;
    children?: React.ReactNode;
}

export function Panel({title, children}: PanelProps) {
    return (
        <div className={"flex flex-col bg-oxford-blue rounded-md p-8 mb-8"}>
            <h2 className={"text-xl mb-4"}>{title}</h2>
            <div className={"w-full h-full bg-navy-blue rounded-md p-8 overflow-scroll scrollbar-hide"}>
                {children}
            </div>
        </div>
    );
}