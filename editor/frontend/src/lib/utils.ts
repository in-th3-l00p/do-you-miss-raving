import {Map, Texture, Tile} from "@/lib/types";
import {Point} from "@/lib/contexts/editor";

export const getTile = (map: Map, selected: Point): Tile => {
    if (map.tiles?.length! < selected.y * map.height + selected.x)
        throw new Error("Tile out of bounds");
    if (!map.tiles)
        throw new Error("No tiles");
    return map.tiles[selected.y * map.height + selected.x];
}

export const getTexture = (map: Map, id: string | undefined): Texture | undefined => {
    if (id === undefined)
        return undefined;
    return map?.textures?.find((texture) => texture._id === id);
}
