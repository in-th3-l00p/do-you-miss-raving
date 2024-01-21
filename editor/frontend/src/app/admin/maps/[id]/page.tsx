"use client";

import "../../style.scss";
import {Map, Texture} from "@/lib/types";
import React, {useEffect, useState} from "react";
import {API} from "@/lib/constants";
import EditorContext, {EditorMode, Point} from "@/lib/contexts/editor";
import {Canvas} from "@/components/Canvas";
import Textures from "@/components/Textures";
import Title from "@/components/Title";
import {Properties} from "@/components/Properties";
import {getTexture} from "@/lib/utils";

export default function MapEditor({ params }: { params: { id: string } }) {
    const [map, setMap] = useState<Map | null>(null);
    const [selectedTile, setSelectedTile] = useState<Point | null>(null);
    const [mode, setMode] = useState<EditorMode>(EditorMode.Select);
    const [images, setImages] = useState<Record<string, HTMLImageElement>>({});

    useEffect(() => {
        fetch(`${API}/api/maps/${params.id}`)
            .then((res) => res.json())
            .then((map) => {
                setMap(map);
            });
    }, []);

    useEffect(() => {
        if (!map)
            return;

        const images: Record<string, HTMLImageElement> = {};
        map.textures?.forEach((texture: Texture) => {
            const img = new Image();
            img.src = `${API}/api/maps/${getTexture(map, texture._id)?.path}`;
            images[texture._id] = img;
        });
        setImages(images);
    }, [map]);

    if (!map || Object.keys(images).length !== map.textures?.length)
        return <p>loading...</p>;
    return (
        <section className={"background flex-grow flex flex-col"}>
            <Title text={"Map: " + map.name} />
            <EditorContext.Provider value={{
                map, setMap,
                selectedTile, setSelectedTile,
                mode, setMode,
                images, setImages
            }}>
                <section
                    className={"grid gap-8 lg:grid-cols-2 p-8 flex-grow"}
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