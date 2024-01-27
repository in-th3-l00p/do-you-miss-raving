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
            <Panel title={"Properties"}>
                <p className={"mb-4"}>Select a tile to see its properties.</p>

                <div className={"flex items-center justify-between gap-4 mb-4"}>
                    <label htmlFor="playerStartX">Player Start X</label>
                    <input
                        type="number"
                        name={"playerStartX"} id={"playerStartX"}
                        className={"input"} defaultValue={map?.playerStartX}
                        onChange={(e) => {
                            if (isNaN(parseFloat(e.target.value)))
                                return;
                            fetch(`${API}/api/maps/${map!._id}/playerCoord`, {
                                method: "PUT",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    x: e.target.value,
                                    y: map?.playerStartY,
                                })
                            })
                                .then(() => setMap( map => ({
                                    ...map!,
                                    playerStartX: parseFloat(e.target.value)
                                })));
                        }}
                    />
                </div>

                <div className={"flex items-center justify-between gap-4 mb-4"}>
                    <label htmlFor="playerStartY">Player Start Y</label>
                    <input
                        type="number"
                        name={"playerStartY"} id={"playerStartY"}
                        className={"input"} defaultValue={map?.playerStartY}
                        onChange={(e) => {
                            if (isNaN(parseFloat(e.target.value)))
                                return;
                            fetch(`${API}/api/maps/${map!._id}/playerCoord`, {
                                method: "PUT",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    x: map?.playerStartX,
                                    y: e.target.value,
                                })
                            })
                                .then(() => setMap( map => ({
                                    ...map!,
                                    playerStartY: parseFloat(e.target.value)
                                })));
                        }}
                    />
                </div>

                <div className={"flex items-center justify-between gap-4 mb-4"}>
                    <label htmlFor="playerStartDirectionX">Player Start Direction X</label>
                    <input
                        type="number"
                        name={"playerStartDirectionX"} id={"playerStartDirectionX"}
                        className={"input"} defaultValue={map?.playerStartDirectionX}
                        onChange={(e) => {
                            if (isNaN(parseFloat(e.target.value)))
                                return;
                            fetch(`${API}/api/maps/${map!._id}/playerDirection`, {
                                method: "PUT",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    x: map?.playerStartDirectionX,
                                    y: e.target.value
                                })
                            })
                                .then(() => setMap( map => ({
                                    ...map!,
                                    playerStartDirectionX: parseFloat(e.target.value)
                                })));
                        }}
                    />
                </div>

                <div className={"flex items-center justify-between gap-4 mb-4"}>
                    <label htmlFor="playerStartDirectionY">Player Start Direction Y</label>
                    <input
                        type="number"
                        name={"playerStartDirectionY"} id={"playerStartDirectionY"}
                        className={"input"} defaultValue={map?.playerStartDirectionY}
                        onChange={(e) => {
                            fetch(`${API}/api/maps/${map!._id}/playerDirection`, {
                                method: "PUT",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    x: e.target.value,
                                    y: map?.playerStartDirectionY,
                                })
                            })
                                .then(() => setMap( map => ({
                                    ...map!,
                                    playerStartDirectionY: parseInt(e.target.value)
                                })));
                        }}
                    />
                </div>

                <div className={"flex items-center justify-between mb-4"}>
                    <label htmlFor={"enemyStart"}>Enemy Start</label>
                    <input
                        type={"checkbox"}
                        className={"w-4 h-4 m-4"}
                        name={"enemyStart"} id={"enemyStart"}
                        defaultChecked={
                            map?.enemyStartX !== undefined &&
                            map?.enemyStartY !== undefined
                        }
                        onChange={(e) => {
                            fetch(`${API}/api/maps/${map!._id}/enemyCoord`, {
                                method: e.target.checked ? "PUT" : "DELETE",
                                headers: {
                                    "Content-Type": "application/json"
                                },
                                body: JSON.stringify({
                                    x: 0, y: 0
                                })
                            })
                                .then(() => setMap( map => ({
                                    ...map!,
                                    enemyStartX: e.target.checked ? 0 : undefined,
                                    enemyStartY: e.target.checked ? 0 : undefined
                                })));
                        }}
                    />
                </div>

                {(map?.enemyStartX !== undefined && map?.enemyStartY !== undefined) && (
                    <>
                        <div className={"flex items-center justify-between gap-4 mb-4"}>
                            <label htmlFor="enemyStartX">Enemy Start X</label>
                            <input
                                type="number"
                                name={"enemyStartX"} id={"enemyStartX"}
                                className={"input"} defaultValue={map?.enemyStartX}
                            />
                        </div>

                        <div className={"flex items-center justify-between gap-4 mb-4"}>
                            <label htmlFor="enemyStartY">Enemy Start Y</label>
                            <input
                                type="number"
                                name={"enemyStartY"} id={"enemyStartY"}
                                className={"input"} defaultValue={map?.enemyStartY}
                            />
                        </div>
                    </>
                )}
            </Panel>
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