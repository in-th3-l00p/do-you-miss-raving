export default function Loading() {
    return (
        <div className={"flex-grow flex flex-col gap-8 justify-center items-center bg-navy-blue"}>
            <h2 className={"text-2xl"}>Loading...</h2>
            <div className="border-t-4 border-oxford-blue rounded-full animate-spin w-10 h-10" />
        </div>
    )
}
