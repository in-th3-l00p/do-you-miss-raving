import type { Config } from 'tailwindcss'

const config: Config = {
  content: [
    './src/pages/**/*.{js,ts,jsx,tsx,mdx}',
    './src/components/**/*.{js,ts,jsx,tsx,mdx}',
    './src/app/**/*.{js,ts,jsx,tsx,mdx}',
  ],
  theme: {
    extend: {
      colors: {
        "oxford-blue": "#04052e",
        "federal-blue": "#140152",
        "navy-blue": "#22007c",
        "duke-blue": "#0d00a4"
      }
    },
  },
  plugins: [],
}
export default config
