import React, {useEffect} from "react";
import EditorContext from "@/lib/contexts/editor";
import {Texture} from "@/lib/types";
import {getTexture, getTile} from "@/lib/utils";
import {API} from "@/lib/constants";

import {Panel} from "@/components/Panel";

interface TextureFieldProps {
    texture?: Texture;
    label: string;
    className?: string;
    onChange?: (id: string) => void;
}

function TextureField({texture, label, className, onChange}: TextureFieldProps) {
    const {map, setMap} = React.useContext(EditorContext);

    return (
        <div className={"flex items-center justify-between gap-4 " + className}>
            <p>{label}</p>

            <div>
                {texture && (
                    <img
                        src={`${API}/api/maps/${texture.path}`}
                        alt={texture.label}
                        className={"w-16 h-16 object-contain me-4 inline"}
                    />
                )}

                <select
                    className={"input"}
                    onChange={(e) => {
                        if (onChange)
                            onChange(e.target.value);
                    }}
                    value={texture?._id || ""}
                >
                    <option value={""}>None</option>
                    {map?.textures?.map((texture, index) => (
                        <option
                            key={index}
                            value={texture._id}
                        >
                            {texture.label}
                        </option>
                    ))}
                </select>
            </div>
        </div>
    );
}

export function Properties() {
    const {map, setMap, selectedTile} = React.useContext(EditorContext);
    const [texture, setTexture] = React.useState<Texture>();
    const [floor, setFloor] = React.useState<Texture>();
    const [ceiling, setCeiling] = React.useState<Texture>();

    useEffect(() => {
        if (!map || !selectedTile)
            return;

        const tile = getTile(map, selectedTile);
        setTexture(() => getTexture(map, tile.texture));
        setFloor(() => getTexture(map, tile.floor));
        setCeiling(() => getTexture(map, tile.ceiling));
    }, [map, selectedTile]);

    if (!map || !selectedTile)
        return (
            <Panel title={"Properties"}/>
        );
    return (
        <Panel title={"Properties"}>
            <TextureField
                label={"Texture"}
                texture={texture}
                className={"mb-4"}
                onChange={(id) => {
                    const tile = getTile(map, selectedTile);
                    tile.texture = id;
                    setMap({...map});

                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${selectedTile.x}&y=${selectedTile.y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "texture": id,
                                "empty": getTile(map, selectedTile).empty
                            })
                        });
                }}
            />
            <TextureField
                label={"Floor"}
                texture={floor}
                className={"mb-4"}
                onChange={(id) => {
                    const tile = getTile(map, selectedTile);
                    tile.floor = id;
                    setMap({...map});

                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${selectedTile.x}&y=${selectedTile.y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "floor": id,
                                "empty": getTile(map, selectedTile).empty
                            })
                        });
                }}
            />
            <TextureField
                label={"Ceiling"}
                texture={ceiling}
                className={"mb-4"}
                onChange={(id) => {
                    const tile = getTile(map, selectedTile);
                    tile.ceiling = id;
                    setMap({...map});

                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${selectedTile.x}&y=${selectedTile.y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "ceiling": id,
                                "empty": getTile(map, selectedTile).empty
                            })
                        });
                }}
            />
            <div className={"flex items-center justify-between"}>
                <p>Empty</p>
                <input
                    type={"checkbox"}
                    className={"w-4 h-4 m-4"}
                    checked={getTile(map, selectedTile).empty}
                    onChange={(e) => {
                        const tile = getTile(map, selectedTile);
                        tile.empty = e.target.checked;
                        setMap({...map});

                        fetch(
                            `${API}/api/maps/${map._id}/tiles?x=${selectedTile.x}&y=${selectedTile.y}`,
                            {
                                method: "PUT",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    "empty": e.target.checked
                                })
                            });
                    }}
                />
            </div>
        </Panel>
    );
}