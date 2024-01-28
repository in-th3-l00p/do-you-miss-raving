import {Map} from "@/lib/types";
import React from "react";
import {Point} from "@/lib/contexts/editor";
import {VisualizeMode} from "@/app/admin/maps/[id]/lib/Canvas";

const TILE_SIZE = 64;
const PLAYER_SIZE = 20;

export function renderMap(
    ctx: CanvasRenderingContext2D,
    map: Map,
    visualizeMode: VisualizeMode,
    images: Record<string, HTMLImageElement>,
    selectedTile?: {x: number, y: number},
) {
    const width = ctx.canvas.width;
    const height = ctx.canvas.height;

    ctx.clearRect(0, 0, width, height);
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, width, height);

    const tileWidth = width / map.width;
    const tileHeight = height / map.height;
    const textureSize = Math.min(tileWidth, tileHeight);

    for (let x = 0; x < map.width; x++) {
        for (let y = 0; y < map.height; y++) {
            const tile = map.tiles![y * map.height + x];
            if (!tile) continue;

            switch (visualizeMode) {
                case VisualizeMode.Texture:
                    if (!tile.empty) {
                        if (tile.texture !== undefined) {
                            ctx.drawImage(images[tile.texture], x * textureSize, y * textureSize, textureSize, textureSize);
                        } else {
                            ctx.fillStyle = "white";
                            ctx.fillRect(x * textureSize, y * textureSize, textureSize, textureSize);
                        }
                    } else {
                        ctx.strokeStyle = "white";
                        ctx.strokeRect(x * textureSize, y * textureSize, textureSize, textureSize);
                    }

                    if (x === selectedTile?.x && y === selectedTile?.y) {
                        ctx.fillStyle = "rgba(255, 255, 255, 0.5)";
                        ctx.fillRect(x * textureSize, y * textureSize, textureSize, textureSize);
                    }
                    break;
                case VisualizeMode.Ceiling:
                    if (tile.ceiling !== undefined) {
                        ctx.drawImage(images[tile.ceiling], x * textureSize, y * textureSize, textureSize, textureSize);
                    } else {
                        ctx.fillStyle = "white";
                        ctx.fillRect(x * textureSize, y * textureSize, textureSize, textureSize);
                    }
                    break;
                case VisualizeMode.Floor:
                    if (tile.floor !== undefined) {
                        ctx.drawImage(images[tile.floor], x * textureSize, y * textureSize, textureSize, textureSize);
                    } else {
                        ctx.fillStyle = "white";
                        ctx.fillRect(x * textureSize, y * textureSize, textureSize, textureSize);
                    }
                    break;
            }
        }
    }

    ctx.fillStyle = "green";
    ctx.arc(
        map.playerStartX * textureSize / TILE_SIZE,
        map.playerStartY * textureSize / TILE_SIZE,
        PLAYER_SIZE * (textureSize / TILE_SIZE),
        0, 2 * Math.PI
    );
    ctx.fill();
    ctx.closePath();

    if (map.enemyStartX !== undefined && map.enemyStartY !== undefined) {
        ctx.fillStyle = "red";
        ctx.arc(
            map.enemyStartX * textureSize / TILE_SIZE,
            map.enemyStartY * textureSize / TILE_SIZE,
            PLAYER_SIZE * (textureSize / TILE_SIZE),
            0, 2 * Math.PI
        );
        ctx.fill();
    }
}

export function setupMapCanvas(
    canvas: HTMLCanvasElement,
    map: Map,
    visualizeMode: VisualizeMode,
    images: Record<string, HTMLImageElement>
) {
    const ctx = canvas.getContext("2d");
    if (!ctx) return () => {};

    const toggleRender = () => {
        if (canvas.parentElement) {
            canvas.width = canvas.parentElement!.clientWidth;
            canvas.height = canvas.parentElement!.clientHeight;
            renderMap(canvas.getContext("2d")!, map, visualizeMode, images);
        }
    }

    if (canvas.parentElement)
        window.addEventListener("resize", toggleRender);

    toggleRender();
    return () => {
        window.removeEventListener("resize", toggleRender);
    }
}

export default function selectMode(
    canvas: HTMLCanvasElement,
    map: Map,
    setSelectedTile: React.Dispatch<React.SetStateAction<Point | null>>,
    onTileClick?: (x: number, y: number) => void
) {
    const onClick = (e: MouseEvent) => {
        const tileSize = canvas.width / map.width;
        const hitX = Math.floor(e.offsetX / tileSize);
        const hitY = Math.floor(e.offsetY / tileSize);
        if (onTileClick)
            onTileClick(hitX, hitY);
    }

    canvas.addEventListener("click", onClick);
    return () => {
        canvas.removeEventListener("click", onClick);
        setSelectedTile(null);
    }
}

export function drawMode(
    canvas: HTMLCanvasElement,
    map: Map,
    onTileDraw?: (x: number, y: number) => void,
    onTileErase?: (x: number, y: number) => void,
    updateMap?: (x: number, y: number) => void
) {
    const onMove = (e: MouseEvent) => {
        if (e.buttons === 0) return;
        if (e.buttons === 1) {
            const tileSize = canvas.width / map.width;
            const hitX = Math.floor(e.offsetX / tileSize);
            const hitY = Math.floor(e.offsetY / tileSize);
            if (onTileDraw) {
                onTileDraw(hitX, hitY);
                if (updateMap)
                    updateMap(hitX, hitY);
            }
        } else if (e.buttons === 4) {
            const tileSize = canvas.width / map.width;
            const hitX = Math.floor(e.offsetX / tileSize);
            const hitY = Math.floor(e.offsetY / tileSize);
            if (onTileErase) {
                onTileErase(hitX, hitY);
                if (updateMap)
                    updateMap(hitX, hitY);
            }
        }
    }
    canvas.addEventListener("mousemove", onMove);
    canvas.addEventListener("mousedown", onMove);
    return () => {
        canvas.removeEventListener("mousemove", onMove);
        canvas.removeEventListener("mousedown", onMove);
    }
}