import type {Metadata, NextComponentType, NextPage} from 'next'
import { Inter } from 'next/font/google'
import './globals.css'
import Link from "next/link";
import React from "react";

const inter = Inter({ subsets: ['latin'] })

const HeaderLink = ({ href, children }: { href: string, children: React.ReactNode }) => {
    return (
        <Link href={href} className={"hover:underline"}>{children}</Link>
    );
}

export const metadata: Metadata = {
  title: 'Create Next App',
  description: 'Generated by create next app',
}

export default function RootLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <html lang="en" className={"min-h-screen"}>
      <body className={inter.className + " min-h-screen flex flex-col"}>
          <header className={"flex items-center bg-oxford-blue justify-between py-8 px-8 sm:px-16 md:px-32 lg:px-64"}>
              <Link href={"/"}>
                  <span className={"text-center text-xl"}>
                      <h2>Do you miss raving?</h2>
                  </span>
              </Link>
              <ul className={"flex items-center gap-4"}>
                  <li><HeaderLink href={"/login"}>Login</HeaderLink></li>
                  <li><HeaderLink href={"/register"}>Register</HeaderLink></li>
              </ul>
          </header>

          {children}
      </body>
    </html>
  )
}
