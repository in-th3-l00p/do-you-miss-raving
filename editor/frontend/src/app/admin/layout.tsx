"use client"

import React, { useState } from "react";
import * as Icon from "react-feather";
import Link from "next/link";

interface SidebarLinkProps {
    title: string;
    children: React.ReactNode;
    href: string;
}

const SidebarLink = ({ title, children, href }: SidebarLinkProps) => {
    return (
        <Link
            href={href}
            className={
                "flex gap-4 items-center py-4 px-8 " +
                "hover:cursor-pointer hover:bg-navy-blue transition-colors "
            }
            title={title}
        >
            {children}
        </Link>
    );
}

const Sidebar = () => {
    const [opened, setOpened] = useState<boolean>(false);

    return (
        <section className={"bg-oxford-blue flex items-start"}>
            {opened && (
                <div>
                    <SidebarLink title={"Dashboard"} href={"/admin"}>
                        <Icon.Activity size={32} />
                        Dashboard
                    </SidebarLink>
                    <SidebarLink title={"Maps"} href={"/admin/maps"}>
                        <Icon.Map size={32} />
                        Maps
                    </SidebarLink>
                </div>
            )}

            <button
                type="button"
                title={"Toggle Sidebar"}
                className={"rounded-md m-4 p-2 hover:bg-navy-blue transition-colors"}
                onClick={() => setOpened(!opened)}
            >
                <Icon.Sidebar size={32} />
            </button>
        </section>
    );
}

interface LayoutProps {
    children: React.ReactNode;
}

export default function Layout({ children }: LayoutProps) {
    return (
        <section className={"flex-grow flex"}>
            <Sidebar />
            {children}
        </section>
    );
}