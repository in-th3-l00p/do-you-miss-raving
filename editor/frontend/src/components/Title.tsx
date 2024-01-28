import React from "react";

export interface TitleProps {
    text: string;
    children?: React.ReactNode;
    className?: string;
}

export default function Title({ children, className }: TitleProps) {
    return (
        <div className={"p-8 h-32 border-b flex justify-between items-center"}>
            <h1 className={"text-2xl md:text-4xl"}>Maps</h1>
            <div className={className}>
                {children}
            </div>
        </div>
    );

}