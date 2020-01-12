module.exports = {
  parser: '@typescript-eslint/parser', // Specifies the ESLint parser
  settings: {
    react: {pragma: 'h'},
  },
  extends: [
    'plugin:react/recommended', // Uses the recommended rules from @eslint-plugin-react,
    'plugin:@typescript-eslint/recommended', // Uses the recommended rules from @typescript-eslint/eslint-plugin
  ],
  parserOptions: {
    ecmaVersion: 2018, // Allows for the parsing of modern ECMAScript features
    sourceType: 'module', // Allows for the use of imports
    ecmaFeatures: {
      jsx: true, // Allows for the parsing of JSX
    },
  },
  rules: {
    // e.g. "@typescript-eslint/explicit-function-return-type": "off",
    '@typescript-eslint/no-unused-vars': [
      'error',
      {
        vars: 'all',
        args: 'none',
        ignoreRestSiblings: false,
      },
    ],
    '@typescript-eslint/explicit-function-return-type': 0,
  },
};
