import React from "react";
import {Map, Sprite} from "@/lib/types";
import {Panel} from "@/components/Panel";
import {API} from "@/lib/constants";
import {TextureField} from "@/app/admin/maps/[id]/lib/properties/TextureField";
import {getTexture} from "@/lib/utils";

interface SpritePropertiesProps {
    map: Map;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;
    selectedSprite: Sprite;
}

export default function SpriteProperties(
    { map, setMap, selectedSprite }: SpritePropertiesProps
) {
    return (
        <Panel title={"Properties"}>
            <div className={"flex items-center justify-between gap-4 mb-4"}>
                <label htmlFor="name">Sprite name</label>
                <input
                    type="text"
                    name={"name"} id={"name"}
                    className={"input"} defaultValue={selectedSprite.name}
                    onChange={(e) => {
                        if (!e.target.value)
                            return;
                        fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                name: e.target.value,
                                texture: selectedSprite.texture,
                                x: selectedSprite.x,
                                y: selectedSprite.y,
                                width: selectedSprite.width,
                                height: selectedSprite.height,
                            })
                        })
                            .then(resp => resp.json())
                            .then(sprites => setMap(map => ({
                                ...map!,
                                sprites
                            })));
                    }}
                />
            </div>

            <div className={"flex items-center justify-between gap-4 mb-4"}>
                <label htmlFor="x">X</label>
                <input
                    type="number"
                    name={"x"} id={"x"}
                    className={"input"} defaultValue={selectedSprite.x}
                    onChange={(e) => {
                        if (isNaN(parseInt(e.target.value)))
                            return;
                        fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                name: selectedSprite.name,
                                texture: selectedSprite.texture,
                                x: e.target.value,
                                y: selectedSprite.y,
                                width: selectedSprite.width,
                                height: selectedSprite.height
                            })
                        })
                            .then(resp => resp.json())
                            .then(sprites => setMap(map => ({
                                ...map!,
                                sprites
                            })));
                    }}
                />
            </div>

            <div className={"flex items-center justify-between gap-4 mb-4"}>
                <label htmlFor="y">Y</label>
                <input
                    type="number"
                    name={"y"} id={"y"}
                    className={"input"} defaultValue={selectedSprite.y}
                    onChange={(e) => {
                        if (isNaN(parseInt(e.target.value)))
                            return;
                        fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                name: selectedSprite.name,
                                texture: selectedSprite.texture,
                                x: selectedSprite.x,
                                y: e.target.value,
                                width: selectedSprite.width,
                                height: selectedSprite.height
                            })
                        })
                            .then(resp => resp.json())
                            .then(sprites => setMap(map => ({
                                ...map!,
                                sprites
                            })));
                    }}
                />
            </div>

            <div className={"flex items-center justify-between gap-4 mb-4"}>
                <label htmlFor="width">Width</label>
                <input
                    type="number"
                    name={"width"} id={"width"}
                    className={"input"} defaultValue={selectedSprite.width}
                    onChange={(e) => {
                        if (isNaN(parseInt(e.target.value)))
                            return;
                        fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                name: selectedSprite.name,
                                texture: selectedSprite.texture,
                                x: selectedSprite.x,
                                y: selectedSprite.y,
                                width: e.target.value,
                                height: selectedSprite.height
                            })
                        })
                            .then(resp => resp.json())
                            .then(sprites => setMap(map => ({
                                ...map!,
                                sprites
                            })));
                    }}
                />
            </div>

            <div className={"flex items-center justify-between gap-4 mb-4"}>
                <label htmlFor="height">Height</label>
                <input
                    type="number"
                    name={"height"} id={"height"}
                    className={"input"} defaultValue={selectedSprite.height}
                    onChange={(e) => {
                        if (isNaN(parseInt(e.target.value)))
                            return;
                        fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                name: selectedSprite.name,
                                texture: selectedSprite.texture,
                                x: selectedSprite.x,
                                y: selectedSprite.y,
                                width: selectedSprite.width,
                                height: e.target.value
                            })
                        })
                            .then(resp => resp.json())
                            .then(sprites => setMap(map => ({
                                ...map!,
                                sprites
                            })));
                    }}
                />
            </div>

            <TextureField
                label={"Texture"}
                texture={getTexture(map, selectedSprite.texture)}
                nullable={false}
                onChange={(id) => {
                    fetch(`${API}/api/maps/${map!._id}/sprites/${selectedSprite._id}`, {
                        method: "PUT",
                        headers: {
                            "Content-Type": "application/json"
                        },
                        body: JSON.stringify({
                            name: selectedSprite.name,
                            texture: id,
                            x: selectedSprite.x,
                            y: selectedSprite.y,
                            width: selectedSprite.width,
                            height: selectedSprite.height
                        })
                    })
                        .then(resp => resp.json())
                        .then(sprites => setMap(map => ({
                            ...map!,
                            sprites
                        })));
                }} />
        </Panel>
    );
}