import { Map } from "../types"
import React from "react";

export type Point = {
    x: number;
    y: number;
}

export enum EditorMode {
    Select,
    Fill,
    Erase,
    Texture,
    Floor,
    Ceiling
}

export interface EditorContextType {
    map: Map | null;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;

    selectedTile: Point | null;
    setSelectedTile: React.Dispatch<React.SetStateAction<Point | null>>;

    mode: EditorMode;
    setMode: React.Dispatch<React.SetStateAction<EditorMode>>;

    images: Record<string, HTMLImageElement>;
    setImages: React.Dispatch<React.SetStateAction<Record<string, HTMLImageElement>>>;
}

const EditorContext = React.createContext<EditorContextType>({
    map: null,
    setMap: () => {},
    selectedTile: null,
    setSelectedTile: () => {},
    mode: EditorMode.Select,
    setMode: () => {},
    images: {},
    setImages: () => {}
});

export default EditorContext;