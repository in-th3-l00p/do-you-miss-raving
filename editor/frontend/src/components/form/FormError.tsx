interface FormErrorProps {
    children: React.ReactNode;
    className?: string;
}

export default function FormError({ children, className }: FormErrorProps) {
    return (
        <p className={"text-red-500 text-sm " + className}>
            {children}
        </p>
    );
}