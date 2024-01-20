import { Map } from "../types"
import React from "react";

export type Point = {
    x: number;
    y: number;
}

export interface EditorContextType {
    map: Map | null;
    setMap: React.Dispatch<React.SetStateAction<Map | null>>;

    selectedTile: Point | null;
    setSelectedTile: React.Dispatch<React.SetStateAction<Point | null>>;
}

const EditorContext = React.createContext<EditorContextType>({
    map: null,
    setMap: () => {},
    selectedTile: null,
    setSelectedTile: () => {}
});

export default EditorContext;