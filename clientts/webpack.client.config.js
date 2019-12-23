/* eslint-disable @typescript-eslint/no-var-requires */
const path = require('path');
const isProd = process.env.NODE_ENV === 'production';

module.exports = [
  {
    entry: './src/entry.tsx',
    mode: isProd ? 'production' : 'development',
    devtool: isProd
      ? 'cheap-module-source-map'
      : 'cheap-module-eval-source-map',
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
      extensions: ['.tsx', '.ts', '.js'],
      modules: ['node_modules', 'src'],
    },
    output: {
      filename: 'bundle.js',
      path: path.resolve(__dirname, 'dist'),
    },
  },
  {
    target: 'node',
    entry: './src/gitService/index.ts',
    mode: isProd ? 'production' : 'development',
    devtool: isProd
      ? 'cheap-module-source-map'
      : 'cheap-module-eval-source-map',
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
      extensions: ['.tsx', '.ts', '.js'],
      modules: ['node_modules', 'src'],
    },
    output: {
      filename: 'gitService.js',
      path: path.resolve(__dirname, 'dist'),
    },
  },
];
