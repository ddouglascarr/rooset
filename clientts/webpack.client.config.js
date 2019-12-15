const path = require('path');
const isProd = process.env.NODE_ENV === 'production';


module.exports = {
  entry: './src/entry.tsx',
  mode: isProd ? 'production' : 'development',
  devtool: isProd ? 'cheap-module-source-map' : 'cheap-module-eval-source-map',
  devServer: {
    contentBase: '../lffrontend/static/js',
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
    path: path.resolve(__dirname, '..', 'lffrontend', 'static', 'js'),
  },
};
