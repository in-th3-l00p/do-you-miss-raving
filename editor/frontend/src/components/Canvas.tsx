import React, {useEffect} from "react";
import EditorContext, {EditorMode} from "@/lib/contexts/editor";
import selectMode, {drawMode, renderMap, setupMapCanvas} from "@/lib/editor";
import {API} from "@/lib/constants";
import {getTile} from "@/lib/utils";
import FormGroup from "@/components/form/FormGroup";
import {Panel} from "@/components/Panel";

export enum VisualizeMode {
    Floor,
    Texture,
    Ceiling
}

export function Canvas() {
    const {map, selectedTile, setSelectedTile, mode, setMode, images} = React.useContext(EditorContext);
    const canvasRef = React.useRef<HTMLCanvasElement>(null);
    const [fillTexture, setFillTexture] = React.useState(map?.textures?.length! > 0 ? map!.textures![0]._id : undefined);
    const [visualizeMode, setVisualizeMode] = React.useState<VisualizeMode>(VisualizeMode.Texture);


    useEffect(() => {
        if (!canvasRef.current || !map)
            return;

        const canvas = canvasRef.current;
        const setupDestructor = setupMapCanvas(canvas, map, visualizeMode, images);

        return () => {
            setupDestructor();
        }
    }, [canvasRef, map]);

    useEffect(() => {
        if (!canvasRef.current || !map)
            return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext("2d");
        if (!ctx)
            return;

        renderMap(ctx, map, visualizeMode, images);
    }, [selectedTile, visualizeMode]);

    useEffect(() => {
        if (!canvasRef.current || !map)
            return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext("2d");
        if (!ctx)
            return;
        let destructor = () => {};
        switch (mode) {
            case EditorMode.Select:
                destructor = selectMode(canvas, map, setSelectedTile, (hitX, hitY) => {
                    setSelectedTile(selectedTile => {
                        if (
                            selectedTile &&
                            selectedTile.x === hitX &&
                            selectedTile.y === hitY
                        )
                            return null;
                        return {x: hitX, y: hitY};
                    })
                });
                break;
            case EditorMode.Fill:
                destructor = drawMode(canvas, map, (x, y) => {
                    getTile(map, {x, y}).empty = false;
                    renderMap(ctx, map, visualizeMode, images);
                }, (x, y) => {
                    getTile(map, {x, y}).empty = true;
                    renderMap(ctx, map, visualizeMode, images);
                }, (x, y) => {
                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${x}&y=${y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "empty": false
                            })
                        });
                });
                break;
            case EditorMode.Erase:
                destructor = drawMode(canvas, map, (x, y) => {
                    getTile(map, {x, y}).empty = true;
                }, (x, y) => {
                    getTile(map, {x, y}).empty = false;
                }, (x, y) => {
                    renderMap(ctx, map, visualizeMode, images);
                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${x}&y=${y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "empty": true
                            })
                        });
                });
                break;
            case EditorMode.Texture:
                destructor = drawMode(canvas, map, (x, y) => {
                    getTile(map, {x, y}).texture = fillTexture;
                }, (x, y) => {
                    getTile(map, {x, y}).texture = undefined;
                }, (x, y) => {
                    renderMap(ctx, map, visualizeMode, images);
                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${x}&y=${y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "texture": fillTexture,
                                "empty": getTile(map, {x, y}).empty
                            })
                        });
                });
                break;
            case EditorMode.Floor:
                destructor = drawMode(canvas, map, (x, y) => {
                    getTile(map, {x, y}).floor = fillTexture;
                }, (x, y) => {
                    getTile(map, {x, y}).floor = undefined;
                }, (x, y) => {
                    renderMap(ctx, map, visualizeMode, images);
                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${x}&y=${y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "floor": fillTexture,
                                "empty": getTile(map, {x, y}).empty
                            })
                        });
                });
                break;
            case EditorMode.Ceiling:
                destructor = drawMode(canvas, map, (x, y) => {
                    getTile(map, {x, y}).ceiling = fillTexture;
                }, (x, y) => {
                    getTile(map, {x, y}).ceiling = undefined;
                }, (x, y) => {
                    renderMap(ctx, map, visualizeMode, images);
                    fetch(
                        `${API}/api/maps/${map._id}/tiles?x=${x}&y=${y}`,
                        {
                            method: "PUT",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                "ceiling": fillTexture,
                                "empty": getTile(map, {x, y}).empty
                            })
                        });
                });
                break;
        }

        return () => {
            destructor();
        }
    }, [mode, fillTexture]);

    useEffect(() => {
        if (!canvasRef.current || !map || !selectedTile)
            return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext("2d");
        if (!ctx)
            return;

        renderMap(ctx, map, visualizeMode, images, selectedTile);
    }, [selectedTile]);

    return (
        <Panel title={"Canvas"}>
            <div className={"p-4 mb-8 w-full bg-oxford-blue rounded-md flex flex-wrap gap-8"}>
                <FormGroup
                    className={"gap-4 items-center"}
                    style={{ flexDirection: "row" }}
                >
                    <label htmlFor="tool">Tool:</label>
                    <select
                        name="tool"
                        id="tool"
                        className={"input w-64"}
                        onChange={(e) => {
                            setMode(parseInt(e.target.value));
                        }}
                    >
                        <option value={EditorMode.Select}>Select</option>
                        <option value={EditorMode.Fill}>Fill</option>
                        <option value={EditorMode.Erase}>Erase</option>
                        {map?.textures?.length! > 0 && (
                            <>
                                <option value={EditorMode.Texture}>Texture</option>
                                <option value={EditorMode.Floor}>Floor</option>
                                <option value={EditorMode.Ceiling}>Ceiling</option>
                            </>
                        )}
                    </select>
                </FormGroup>
                <FormGroup
                    className={"gap-4 items-center"}
                    style={{ flexDirection: "row" }}
                >
                    <label htmlFor="visualize">Visualize:</label>
                    <select
                        name="visualize"
                        id="visualize"
                        className={"input w-64"}
                        onChange={(e) => {
                            setVisualizeMode(parseInt(e.target.value));
                        }}
                        defaultValue={visualizeMode}
                    >
                        <option value={VisualizeMode.Floor}>Floor</option>
                        <option value={VisualizeMode.Texture}>Texture</option>
                        <option value={VisualizeMode.Ceiling}>Ceiling</option>
                    </select>
                </FormGroup>
                {(
                    mode === EditorMode.Texture ||
                    mode === EditorMode.Floor ||
                    mode === EditorMode.Ceiling
                ) && (
                    <FormGroup
                        className={"gap-4 items-center"}
                        style={{ flexDirection: "row" }}
                    >
                        <label htmlFor="texture">Texture:</label>
                        <select
                            name="texture"
                            id="texture"
                            className={"input w-64"}
                            onChange={(e) => {
                                setFillTexture(e.target.value);
                            }}
                            defaultValue={fillTexture}
                        >
                            {map?.textures?.map((texture, index) => (
                                <option
                                    key={index}
                                    value={texture._id}
                                >
                                    {texture.label}
                                </option>
                            ))}
                        </select>
                    </FormGroup>
                )}
            </div>
            <div className={"relative aspect-square w-full max-w-[500px] mx-auto my-auto bg-red-700"}>
                <canvas
                    className={"absolute top-0 left-0"}
                    ref={canvasRef}
                />
            </div>
        </Panel>
    );
}
