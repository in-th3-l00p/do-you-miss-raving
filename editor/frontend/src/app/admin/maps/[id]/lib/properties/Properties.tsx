import React from "react";
import EditorContext from "@/lib/contexts/editor";

import {Panel} from "@/components/Panel";
import {MapProperties} from "@/app/admin/maps/[id]/lib/properties/MapProperties";
import {TileProperties} from "@/app/admin/maps/[id]/lib/properties/TileProperties";
import SpriteProperties from "@/app/admin/maps/[id]/lib/properties/SpriteProperties";

export function Properties() {
    const {map, setMap, selectedTile, selectedSprite} = React.useContext(EditorContext);

    if (!map)
        return <Panel title={"Properties"} />;
    if (!selectedTile && !selectedSprite)
        return <MapProperties
            map={map}
            setMap={setMap}
        />;
    if (selectedSprite)
        return <SpriteProperties
            map={map}
            setMap={setMap}
            selectedSprite={selectedSprite}
        />
    return <TileProperties
        map={map}
        setMap={setMap}
        selectedTile={selectedTile!}
    />;
}