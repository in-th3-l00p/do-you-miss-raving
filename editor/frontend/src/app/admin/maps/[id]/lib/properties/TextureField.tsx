import {Texture} from "@/lib/types";
import React from "react";
import EditorContext from "@/lib/contexts/editor";
import {API} from "@/lib/constants";

interface TextureField {
    texture?: Texture;
    label: string;
    className?: string;
    onChange?: (id: string) => void;
    nullable?: boolean;
}

export function TextureField({texture, label, className, onChange, nullable = true}: TextureField) {
    const {map} = React.useContext(EditorContext);

    return (
        <div className={"flex items-center justify-between gap-4 " + className}>
            <p>{label}</p>

            <div>
                {texture && (
                    <img
                        src={`${API}/api/maps/${texture.path}`}
                        alt={texture.label}
                        className={"w-16 h-16 object-contain me-4 inline"}
                    />
                )}

                <select
                    className={"input"}
                    onChange={(e) => {
                        if (onChange)
                            onChange(e.target.value);
                    }}
                    defaultValue={texture?._id || ""}
                >
                    {nullable && <option value={""}>None</option>}
                    {map?.textures?.map((texture, index) => (
                        <option
                            key={index}
                            value={texture._id}
                        >
                            {texture.label}
                        </option>
                    ))}
                </select>
            </div>
        </div>
    );
}