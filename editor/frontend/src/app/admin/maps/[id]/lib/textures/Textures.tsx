import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {API} from "@/lib/constants";
import * as Icon from "react-feather";
import {Panel} from "@/components/Panel";
import {UploadModal} from "@/app/admin/maps/[id]/lib/textures/UploadModal";
import {Texture} from "@/lib/types";
import EditModal from "@/app/admin/maps/[id]/lib/textures/EditModal";

export default function Textures() {
    const {map, setMap} = React.useContext(EditorContext);
    const [uploadOpened, setUploadOpened] = React.useState(false);

    const [selectedTexture, setSelectedTexture] = React.useState<Texture | null>(null);

    if (!map)
        return (
            <Panel title={"Textures"}/>
        );
    return (
        <>
            <UploadModal
                opened={uploadOpened}
                setOpened={setUploadOpened}
            />
            {selectedTexture && (
                <EditModal
                    map={map}
                    texture={selectedTexture}
                    setMap={setMap}
                    open={!!selectedTexture}
                    setOpened={(opened) => {
                        if (!opened)
                            setSelectedTexture(null);
                    }}
                />
            )}
            <Panel title={"Textures"}>
                <div className={"flex flex-wrap gap-8"}>
                    {map.textures?.map((texture, index) => (
                        <button
                            type={"button"}
                            key={index}
                            className={
                                "text-center p-4 bg-oxford-blue rounded-md w-48 " +
                                "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                            }
                            onClick={() => setSelectedTexture(texture)}
                        >
                            <img
                                src={`${API}/api/maps/${texture.path}`}
                                alt={texture.label}
                                className={"aspect-square object-contain w-32 mx-auto my-auto"}
                            />
                            <p className={"mt-4"}>{texture.label}</p>
                        </button>
                    ))}

                    <button
                        type={"button"}
                        className={
                            "flex justify-center items-center bg-oxford-blue aspect-square w-48 rounded-md shadow-md " +
                            "hover:bg-darker-oxford-blue hover:shadow-xl transition-all"
                        }
                        onClick={() => {
                            setUploadOpened(true);
                        }}
                    >
                        <Icon.Plus size={"80px"}/>
                    </button>
                </div>
            </Panel>
        </>
    );
}