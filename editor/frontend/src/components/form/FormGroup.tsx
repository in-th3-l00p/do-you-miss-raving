interface FormGroupProps {
    children?: React.ReactNode;
    className?: string;
}

export default function FormGroup({ className, children }: FormGroupProps) {
    return (
        <div className={"w-full flex flex-col max-w-[600px] " + className}>
            {children}
        </div>
    );
}