import resolve from 'rollup-plugin-node-resolve';
import typescript from 'rollup-plugin-typescript2';
import pkg from './package.json';

const pageOpts = {
  external: ['preact'],
  plugins: [
    resolve(),
    typescript({
      typescript: require('typescript'),
    }),
  ],
};

const pageOutputOpts = {
  format: 'umd',
  globals: {
    preact: 'preact',
  },
};

export default [
  {
    input: 'src/pages/WelcomePage.tsx',
    output: [
      {
        file: 'dist/WelcomePage.js',
        name: 'WelcomePage',
        ...pageOutputOpts,
      },
    ],
    ...pageOpts,
  },
  {
    input: 'src/pages/LoginPage.tsx',
    output: [
      {
        file: 'dist/LoginPage.js',
        name: 'LoginPage',
        ...pageOutputOpts,
      },
    ],
    ...pageOpts,
  },
];
