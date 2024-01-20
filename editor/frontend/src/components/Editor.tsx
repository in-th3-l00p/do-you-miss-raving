import React, {useEffect} from "react";
import EditorContext from "@/lib/contexts/editor";
import {renderMap, setupMapCanvas} from "@/lib/editor";
import {Texture} from "@/lib/types";
import {API} from "@/lib/constants";
import {getTexture, getTile} from "@/lib/utils";

export interface PanelProps {
    title: string;
    children?: React.ReactNode;
}

export function Panel({title, children}: PanelProps) {
    return (
        <div className={"flex flex-col bg-oxford-blue rounded-md p-8 mb-8"}>
            <h2 className={"text-xl mb-4"}>{title}</h2>
            <div className={"w-full h-full bg-navy-blue rounded-md p-8"}>
                {children}
            </div>
        </div>
    );
}

export function Canvas() {
    const {map, selectedTile, setSelectedTile} = React.useContext(EditorContext);

    const canvasRef = React.useRef<HTMLCanvasElement>(null);

    useEffect(() => {
        if (!canvasRef.current || !map)
            return;

        const canvas = canvasRef.current;
        const destructor = setupMapCanvas(canvas, map, (hitX, hitY) => {
            if (
                selectedTile &&
                selectedTile.x === hitX &&
                selectedTile.y === hitY
            ) {
                setSelectedTile(null);
                return;
            }
            setSelectedTile({x: hitX, y: hitY});
        });

        return () => {
            destructor();
        }
    }, [canvasRef, map, selectedTile]);

    useEffect(() => {
        if (!canvasRef.current || !map || !selectedTile)
            return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext("2d");
        if (!ctx)
            return;

        renderMap(ctx, map, selectedTile);
    }, [selectedTile]);

    return (
        <Panel title={"Canvas"}>
            <div className={"relative aspect-square w-full max-w-[500px] mx-auto my-auto bg-red-700"}>
                <canvas
                    className={"absolute top-0 left-0"}
                    ref={canvasRef}
                />
            </div>
        </Panel>
    );
}

interface TextureFieldProps {
    texture?: Texture;
    label: string;
    className?: string;
    onChange?: (id: string) => void;
}

function TextureField({ texture, label, className, onChange }: TextureFieldProps) {
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
            <Panel title={"Properties"} />
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

