import {Map} from "@/lib/types";

export function renderMap(
    ctx: CanvasRenderingContext2D,
    map: Map,
    selectedTile?: {x: number, y: number}
) {
    const width = ctx.canvas.width;
    const height = ctx.canvas.height;

    ctx.clearRect(0, 0, width, height);
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, width, height);

    const tileWidth = width / map.width;
    const tileHeight = height / map.height;

    for (let x = 0; x < map.width; x++) {
        for (let y = 0; y < map.height; y++) {
            const tile = map.tiles![y * map.height + x];
            if (!tile) continue;

            if (!tile.empty) {
                ctx.fillStyle = "red";
                ctx.fillRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            } else {
                ctx.strokeStyle = "white";
                ctx.strokeRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            }

            if (x === selectedTile?.x && y === selectedTile?.y) {
                ctx.fillStyle = "rgba(255, 255, 255, 0.5)";
                ctx.fillRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            }
        }
    }
}

export function setupMapCanvas(
    canvas: HTMLCanvasElement,
    map: Map,
    onTileClick?: (x: number, y: number) => void
) {
    const ctx = canvas.getContext("2d");
    if (!ctx) return () => {};

    const toggleRender = () => {
        if (canvas.parentElement) {
            canvas.width = canvas.parentElement!.clientWidth;
            canvas.height = canvas.parentElement!.clientHeight;
            renderMap(canvas.getContext("2d")!, map);
        }
    }

    const onClick = (e: MouseEvent) => {
        const tileSize = canvas.width / map.width;
        const hitX = Math.floor(e.offsetX / tileSize);
        const hitY = Math.floor(e.offsetY / tileSize);
        if (onTileClick)
            onTileClick(hitX, hitY);
    }

    if (canvas.parentElement)
        window.addEventListener("resize", toggleRender);
    canvas.addEventListener("click", onClick);

    toggleRender();
    return () => {
        window.removeEventListener("resize", toggleRender);
        canvas.removeEventListener("click", onClick);
    }
}
