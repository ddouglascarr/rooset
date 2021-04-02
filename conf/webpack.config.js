/* eslint-disable @typescript-eslint/no-var-requires */
const path = require('path');
const isProd = process.env.NODE_ENV === 'production';

const baseConfig = {
  mode: isProd ? 'production' : 'development',
  devtool: isProd ? 'cheap-module-source-map' : 'cheap-module-eval-source-map',
  // webpack dev server only reads the first one of these in the array of configs
  devServer: {
    contentBase: './dist',
    writeToDisk: true, // so we can watch the serverside ones on the disk
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        loader: 'ts-loader',
        exclude: /node_modules/,
      },
    ],
  },
  resolve: {
    extensions: ['.tsx', '.ts', '.jsx', '.js'],
    modules: ['node_modules', '..'],
    alias: {
      react: 'preact/compat',
      'react-dom/test-utils': 'preact/test-utils',
      'react-dom': 'preact/compat',
    },
  },
};

module.exports = [
  {
    ...baseConfig,
    entry: './initiativeclient/newInitiativeEntry.tsx',
    output: {
      filename: 'newInitiative.js',
      path: path.resolve(__dirname, '..', 'dist'),
      publicPath: 'http://localhost:8082/',
    },
  },
  {
    ...baseConfig,
    entry: './initiativeclient/competingInitiativeEntry.tsx',
    output: {
      filename: 'competingInitiative.js',
      path: path.resolve(__dirname, '..', 'dist'),
      publicPath: 'http://localhost:8082/',
    },
  },
];
