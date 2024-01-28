import React from "react";

export default function Loading() {
    return (
        <div className={"flex-grow w-screen flex justify-center items-center flex-col gap-16 background"}>
            <h1 className={"text-3xl"}>Loading...</h1>
            <div
                className={"animate-spin rounded-full h-32 w-32 border-[20px] border-t-duke-blue border-b-duke-blue border-transparent"}
            />
        </div>
    )
}