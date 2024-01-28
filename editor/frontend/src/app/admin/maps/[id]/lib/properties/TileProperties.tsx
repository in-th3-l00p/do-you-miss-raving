import {Map, Texture} from "@/lib/types";
import React, {useEffect} from "react";
import {getTexture, getTile} from "@/lib/utils";
import {Panel} from "@/components/Panel";
import {TextureField} from "@/app/admin/maps/[id]/lib/properties/TextureField";
import {API} from "@/lib/constants";

interface TilePropertiesProps {
    map: Map;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;
    selectedTile: { x: number, y: number };
}

export function TileProperties({map, setMap, selectedTile}: TilePropertiesProps) {
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
                    defaultChecked={getTile(map, selectedTile).empty}
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