import {Map} from "@/lib/types";
import React from "react";
import {Panel} from "@/components/Panel";
import {API} from "@/lib/constants";

interface MapPropertiesProps {
    map: Map;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;
}

export function MapProperties({map, setMap}: MapPropertiesProps) {
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
                            .then(() => setMap(map => ({
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
                            .then(() => setMap(map => ({
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
                            .then(() => setMap(map => ({
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
                            .then(() => setMap(map => ({
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
                            .then(() => setMap(map => ({
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
                            onChange={(e) => {
                                if (isNaN(parseFloat(e.target.value)))
                                    return;
                                fetch(`${API}/api/maps/${map!._id}/enemyCoord`, {
                                    method: "PUT",
                                    headers: {
                                        "Content-Type": "application/json"
                                    },
                                    body: JSON.stringify({
                                        x: e.target.value, y: map?.enemyStartY
                                    })
                                })
                                    .then(() => setMap(map => ({
                                        ...map!,
                                        enemyStartX: parseFloat(e.target.value),
                                        enemyStartY: map?.enemyStartY
                                    })));
                            }}
                        />
                    </div>

                    <div className={"flex items-center justify-between gap-4 mb-4"}>
                        <label htmlFor="enemyStartY">Enemy Start Y</label>
                        <input
                            type="number"
                            name={"enemyStartY"} id={"enemyStartY"}
                            className={"input"} defaultValue={map?.enemyStartY}
                            onChange={(e) => {
                                if (isNaN(parseFloat(e.target.value)))
                                    return;
                                fetch(`${API}/api/maps/${map!._id}/enemyCoord`, {
                                    method: "PUT",
                                    headers: {
                                        "Content-Type": "application/json"
                                    },
                                    body: JSON.stringify({
                                        y: e.target.value, x: map?.enemyStartX
                                    })
                                })
                                    .then(() => setMap(map => ({
                                        ...map!,
                                        enemyStartY: parseFloat(e.target.value),
                                        enemyStartX: map?.enemyStartX
                                    })));
                            }}
                        />
                    </div>
                </>
            )}
        </Panel>
    );
}