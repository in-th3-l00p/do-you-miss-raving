import "../style.scss";
import { Map } from "@/lib/types";
import React from "react";
import Link from "next/link";
import {API} from "@/lib/constants";

export default async function Maps() {
    const maps: Map[] = await fetch(`${API}/api/maps`).then((res) => res.json());

    return (
        <div className={"background flex-grow"}>
            <h1 className={"p-8 title border-b mb-8"}>Maps</h1>
            {maps.map((map, index) => (
                <Link
                    key={index}
                    href={"/admin/maps/" + map._id}
                    className={
                        "flex justify-between mx-auto " +
                        "m-8 p-8 rounded-md block bg-navy-blue max-w-[800px] " +
                        "hover:cursor-pointer hover:bg-duke-blue transition-all"
                    }
                >
                    <div>
                        <h2 className={"text-2xl"}>{map.name}</h2>
                        <p className={"text-slate-400 font-light"}>{map.width} x {map.height}</p>
                    </div>
                </Link>
            ))}
        </div>
    );
}
