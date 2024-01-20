"use client";

import "../../style.scss";
import {Map} from "@/lib/types";
import React, {useEffect, useState} from "react";
import {API} from "@/lib/constants";
import EditorContext, {Point} from "@/lib/contexts/editor";
import {Canvas, Properties} from "@/components/Editor";
import Textures from "@/components/Textures";


export default function MapEditor({ params }: { params: { id: string } }) {
    const [map, setMap] = useState<Map | null>(null);
    const [selectedTile, setSelectedTile] = useState<Point | null>(null);

    useEffect(() => {
        fetch(`${API}/api/maps/${params.id}`)
            .then((res) => res.json())
            .then((map) => setMap(map));
    }, []);

    if (!map) return <p>loading...</p>;
    return (
        <section className={"background flex-grow flex flex-col"}>
            <h1 className={"p-8 title border-b"}>Map: <span>{map.name}</span></h1>
            <EditorContext.Provider value={{ map, setMap, selectedTile, setSelectedTile }}>
                <section
                    className={"grid gap-8 md:grid-cols-2 p-8 flex-grow"}
                    style={{ background: "rgba(255, 255, 255, 0.1)" }}
                >
                    <Canvas />
                    <Properties />
                    <Textures />
                </section>
            </EditorContext.Provider>
        </section>
    );
}