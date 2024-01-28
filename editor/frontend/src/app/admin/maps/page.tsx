"use client"

import {Map} from "@/lib/types";
import React, {useEffect, useState} from "react";
import Link from "next/link";
import {API} from "@/lib/constants";
import Title from "@/components/Title";
import * as Icon from "react-feather";
import Loading from "@/components/Loading";

export default async function Maps() {
    const [maps, setMaps] = useState<Map[]>();
    useEffect(() => {
        fetch(`${API}/api/maps`, {
            "cache": "no-cache"
        })
            .then((res) => res.json())
            .then((maps) => setMaps(maps));
    }, []);

    if (maps === undefined)
        return <Loading />
    return (
        <div className={"background flex-grow"}>
            <Title text={"Maps"}>
                <a href={"/admin/maps/create"} className={"btn block aspect-square"}>
                    <Icon.Plus size={"32"} />
                </a>
            </Title>
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
