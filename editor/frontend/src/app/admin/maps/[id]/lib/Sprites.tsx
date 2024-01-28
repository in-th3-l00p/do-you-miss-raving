import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {Panel} from "@/components/Panel";
import {API} from "@/lib/constants";
import * as Icon from "react-feather";
import {getTexture} from "@/lib/utils";

export default function Sprites() {
    const {map, setMap, setSelectedTile, selectedSprite, setSelectedSprite} = React.useContext(EditorContext);

    return (
        <>
            <Panel title={"Sprites"}>
                <div className={"flex flex-wrap gap-8"}>
                    {map?.sprites?.map((sprite, index) => (
                        <button
                            type={"button"}
                            key={index}
                            className={
                                "text-center p-4 rounded-md w-48 " +
                                "hover:bg-darker-oxford-blue hover:shadow-xl transition-all " +
                                (selectedSprite && selectedSprite._id === sprite._id ? "bg-darker-oxford-blue" : "bg-oxford-blue")
                            }
                            onClick={() => {
                                if (selectedSprite && selectedSprite._id === sprite._id)
                                    setSelectedSprite(null);
                                else {
                                    setSelectedSprite(sprite);
                                    setSelectedTile(null);
                                }
                            }}
                        >
                            <img
                                src={`${API}/api/maps/${getTexture(map, sprite.texture)!.path}`}
                                alt={getTexture(map, sprite.texture)!.label}
                                className={"aspect-square object-contain w-32 mx-auto my-auto"}
                            />
                            <p className={"mt-4"}>{sprite.name}</p>
                        </button>
                    ))}

                    {map?.textures?.length !== 0 && (
                        <button
                            type={"button"}
                            className={
                                "flex justify-center items-center bg-oxford-blue aspect-square w-48 rounded-md shadow-md " +
                                "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                            }
                            onClick={() => {
                                fetch(`${API}/api/maps/${map!._id}/sprites`, {
                                    method: "POST",
                                    headers: {
                                        "Content-Type": "application/json"
                                    },
                                    body: JSON.stringify({
                                        name: getTexture(map!, map!.textures![0]._id)!.label,
                                        texture: map!.textures![0]._id,
                                        x: 0,
                                        y: 0,
                                        width: 1,
                                        height: 1,
                                    })
                                })
                                    .then(resp => resp.json())
                                    .then(sprite => setMap(map => ({
                                        ...map!,
                                        sprites: sprite
                                    })));
                            }}
                        >
                            <Icon.Plus size={"80px"}/>
                        </button>
                    )}
                </div>
            </Panel>
        </>
    );
}
