import Image from 'next/image'
import style from "./style.module.scss";

export default function Home() {
  return (
      <section className={style.hero}>
          <div className={"p-8 text-center md:text-left"}>
              <h1 className={"text-3xl font-bold mb-4"}>Do you miss raving?</h1>
              <p>engine made from scratch in C++, using SFML as a graphics library</p>
          </div>
          <div
              className={"relative flex justify-center md:items-center"}
              style={{ width: "500px", height: "400px" }}
          >
              <Image
                  src="/screenshot.png"
                  width={500}
                  height={500}
                  alt="screen shot of the engine"
                  className={"rounded-lg shadow w-64 h-40 md:w-96 md:h-60"}
              />
              <Image
                  src={"/logo.png"}
                  width={500}
                  height={500}
                  alt="logo"
                  className={"absolute right-20 md:right-10 top-20 md:top-60 rounded-lg shadow w-32 h-32"}
              />
          </div>
      </section>
  )
}
