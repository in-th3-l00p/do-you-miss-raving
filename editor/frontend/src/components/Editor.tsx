import React, {useEffect} from "react";
import EditorContext, {Point} from "@/lib/contexts/editor";
import {renderMap, setupMapCanvas} from "@/lib/editor";
import {Map, Texture, Tile} from "@/lib/types";
import {API} from "@/lib/constants";

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
}

function TextureField({ texture, label, className }: TextureFieldProps) {
    return (
        <div className={"flex items-center justify-between gap-4 " + className}>
            <p>{label}</p>

            {texture ? (
                <img
                    src={`${API}/api/maps/${texture.path}`}
                    alt={texture.label}
                />
            ): (
                <button
                    type={"button"}
                    className={"btn"}
                >
                    upload
                </button>
            )}
        </div>
    );
}

export function Properties() {
    const {map, setMap, selectedTile} = React.useContext(EditorContext);
    const getTile = (map: Map, selected: Point): Tile => {
        if (map.tiles?.length! < selected.y * map.height + selected.x)
            throw new Error("Tile out of bounds");
        if (!map.tiles)
            throw new Error("No tiles");
        return map.tiles[selected.y * map.height + selected.x];
    }

    if (!map || !selectedTile)
        return (
            <Panel title={"Properties"} />
        );
    return (
        <Panel title={"Properties"}>
            <TextureField
                label={"Texture"}
                texture={getTile(map, selectedTile).texture}
                className={"mb-4"}
            />
            <TextureField
                label={"Floor"}
                texture={getTile(map, selectedTile).floor}
                className={"mb-4"}
            />
            <TextureField
                label={"Ceiling"}
                texture={getTile(map, selectedTile).ceiling}
                className={"mb-4"}
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

