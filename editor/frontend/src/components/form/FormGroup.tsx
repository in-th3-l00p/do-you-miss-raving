interface FormGroupProps {
    children?: React.ReactNode;
    className?: string;
    style?: React.CSSProperties;
}

export default function FormGroup({ className, children, style }: FormGroupProps) {
    return (
        <div
            className={"w-full flex flex-col max-w-[600px] " + (className ? className : "")}
            style={style}
        >
            {children}
        </div>
    );
}