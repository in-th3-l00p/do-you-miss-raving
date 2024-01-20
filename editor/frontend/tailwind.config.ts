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
        "darker-oxford-blue": "#010113",
        "oxford-blue": "#04052e",
        "federal-blue": "#140152",
        "navy-blue": "#1c006e",
        "duke-blue": "#0c0080"
      }
    },
  },
  plugins: [],
}
export default config
